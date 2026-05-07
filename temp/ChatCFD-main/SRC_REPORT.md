# ChatCFD `src/` 源码详细解析报告

---

## 目录

1. [模块依赖关系总览](#一模块依赖关系总览)
2. [config.py — 全局配置与状态中枢](#二configpy--全局配置与状态中枢)
3. [chatbot.py — 系统主入口与交互层](#三chatbotpy--系统主入口与交互层)
4. [qa_modules.py — LLM 调用封装层](#四qa_modulespy--llm-调用封装层)
5. [pdf_chunk_ask_question.py — PDF 语义检索（RAG）](#五pdf_chunk_ask_questionpy--pdf-语义检索rag)
6. [file_preparation.py — OpenFOAM 文件生成器](#六file_preparationpy--openfoam-文件生成器)
7. [main_run_chatcfd.py — 主调度与仿真循环](#七main_run_chatcfdpy--主调度与仿真循环)
8. [file_corrector.py — 自动错误修正引擎](#八file_correctorpy--自动错误修正引擎)
9. [Reflextion.py — 元认知反思机制](#九refleXtionpy--元认知反思机制)
10. [run_of_case.py — OpenFOAM 仿真执行器](#十run_of_casepy--openfoam-仿真执行器)
11. [file_summary.py — 案例摘要生成器](#十一file_summarypy--案例摘要生成器)
12. [preprocess_OF_tutorial.py — 知识库构建工具（离线）](#十二preprocess_of_tutorialpy--知识库构建工具离线)
13. [case_file_requirements.py — 网格边界解析（旧版）](#十三case_file_requirementspy--网格边界解析旧版)

---

## 一、模块依赖关系总览

```
chatbot.py (入口)
    ├── config.py                       ← 所有模块的全局状态源
    ├── qa_modules.py                   ← LLM 调用（被所有需要AI的模块引用）
    ├── preprocess_OF_tutorial.py       ← 加载知识库
    ├── file_preparation.py
    │       ├── pdf_chunk_ask_question.py  ← RAG检索
    │       ├── qa_modules.py
    │       └── file_corrector.py       ← 部分工具函数
    └── main_run_chatcfd.py
            ├── file_preparation.py
            ├── file_corrector.py
            │       └── qa_modules.py
            ├── run_of_case.py
            └── Reflextion.py
                    └── qa_modules.py

file_summary.py  ← 仿真完成后由 chatbot.py 调用
```

**调用关系要点：**
- `config.py` 是所有模块的"全局变量池"，模块之间通过修改/读取 `config.*` 共享运行时状态
- `qa_modules.py` 是唯一与 DeepSeek API 通信的模块，其他模块均通过它发起 LLM 请求
- `file_corrector.py` 同时承担工具函数库（被 `file_preparation.py` 导入）和独立错误修正器（被 `main_run_chatcfd.py` 调用）两个角色

---

## 二、`config.py` — 全局配置与状态中枢

**文件大小：** ~474 行  
**核心职责：** 系统级配置加载、全局状态共享、OpenFOAM 环境初始化

### 2.1 配置数据类（dataclass）

| 数据类 | 字段 | 说明 |
|---|---|---|
| `path_config` | `root_dir`, `src_dir`, `output_dir`, `database_dir`, `temp_dir`, `document_path`, `grid_path`, `output_case_path` | 所有文件路径，`__post_init__` 自动推导依赖路径 |
| `dependencies_config` | `OpenFOAM_path`, `OpenFOAM_tutorials_path`, `sentence_transformer_path` | 外部软件和模型路径 |
| `llm_config` | `DEEPSEEK_R1_KEY/BASE_URL/temperature`, `DEEPSEEK_V3_KEY/BASE_URL/temperature` | DeepSeek 双模型 API 配置 |
| `run_config` | `mode`, `grid_type`, `run_time`, `max_running_test_round` | 运行模式与迭代控制 |
| `pdf_config` | `pdf_chunk_d` | PDF 语义检索的相似度阈值 |
| `case_status` | `case_name`, `case_solver`, `turbulence_model`, `file_structure`, `case_info_simple`, `simulation_requirement` | 当前仿真案例的运行时状态 |
| `grid_status` | `grid_bc_name`, `grid_boundary_conditions`, `grid_boundary_init`, `field_ic_bc_from_input` | 网格文件的边界信息 |

### 2.2 `ConfigManager` 类

负责统一的配置生命周期管理：

```python
config_manager = ConfigManager()
config_manager.load_config()       # 从 chatcfd_config.json 读取所有配置
config_manager.load_openfoam_env() # source /usr/lib/openfoam/openfoam2406/etc/bashrc 并注入环境变量
```

`load_config()` 将 API Key 等敏感信息注入 `os.environ`，确保子进程（OpenFOAM 求解器）也能访问。  
`load_openfoam_env()` 通过 `subprocess` 调用 Bash `source` 命令，将 OpenFOAM 完整环境变量注入当前 Python 进程，是 `fluentMeshToFoam`、求解器等命令能被调用的前提。

### 2.3 全局变量（模块级）

以下为跨模块共享的关键运行时变量：

| 变量名 | 类型 | 说明 |
|---|---|---|
| `path_cfg` | `path_config` 实例 | 当前运行的路径配置 |
| `case_info` | `case_status` 实例 | 当前案例的状态（求解器、湍流模型等） |
| `grid_info` | `grid_status` 实例 | 当前网格边界信息 |
| `error_history` | `list` | 历次运行错误的记录，用于判断错误是否重复 |
| `correct_trajectory` | `list[dict]` | 历次修正操作的轨迹 `{文件名: [原内容, 修改后内容]}` |
| `OF_case_data_dict` | `dict` | 当前会话加载的知识库数据（仅含目标求解器相关案例） |
| `global_files` | `dict` | 当前待生成/已生成的 OpenFOAM 文件字典 `{路径: 内容}` |
| `solver_keywords` | `list[str]` | OpenFOAM v2406 全部已知求解器名称（80+个） |
| `turbulence_model_keywords` | `list[str]` | 支持的湍流模型名称列表（17个） |
| `incompressible_solvers` / `compressible_solvers` | `list[str]` | 按可压/不可压分类的求解器列表，用于维度自动判断 |
| `steady_solvers` | `list[str]` | 稳态求解器列表，用于 CFL 控制设置 |
| `pdf_chunk_d` | `float` | FAISS 距离阈值（1.5），超过此距离的文本块不纳入 RAG 上下文 |

### 2.4 模块加载时的副作用

`config.py` 被 `import` 时立即执行：
1. 实例化 `ConfigManager` 并读取 `inputs/chatcfd_config.json`
2. 将所有 API Key 写入环境变量
3. 调用 `load_openfoam_env()` 注入 OpenFOAM 环境变量
4. 确保 `database_OFv24/`、`run_chatcfd/`、`temp/` 目录存在

---

## 三、`chatbot.py` — 系统主入口与交互层

**文件大小：** ~808 行  
**核心职责：** Web UI / CLI 双模式入口、用户对话管理、案例选择确认、调度 `main_run_chatcfd`

### 3.1 内置 Prompt 常量

| 常量 | 用途 |
|---|---|
| `SYSTEM_PROMPT` | ChatBot 的系统角色设定（礼貌专业的 CFD 助手） |
| `EXTRACT_CASES_FROM_TEXT_PROMPT` | 要求 LLM 从文档中列举所有 CFD 案例，每个案例标注 Case_X |
| `ASK_TO_CHOOSE_CASE_AND_SOLVER` | 引导用户选择具体案例和求解器 |
| `CHOOSE_A_CASE_TO_RUN` | 将用户自然语言选择转化为结构化 JSON（含求解器、湍流模型等字段） |
| `CONFIRM_SIMULATION_REQUIREMENT` | 展示配置摘要，请求用户确认 |
| `CONVERT_JSON_TO_MD` | 将 JSON 格式的配置转换为 Markdown 便于显示 |

### 3.2 `ChatBot` 类

负责维护对话历史和调用 DeepSeek R1 API：

```python
class ChatBot:
    def process_document(self, file, type)  # 用 PyPDF2 提取 PDF/TXT 文本
    def get_response(self, messages)        # 调用 R1 模型，返回文本
    def convert_json_to_md(self, json_str)  # JSON → Markdown（调用 R1）
    def count_tokens(self, text)            # tiktoken 估算 Token 数
    def create_zip_archive(self, src, out)  # 将案例目录打包为 .zip 供下载
```

### 3.3 Web 模式：`main()` 函数（Streamlit）

使用 `st.session_state` 管理完整的用户会话状态，共 11 个状态标志位：

| 状态标志 | 含义 |
|---|---|
| `document_processed` | PDF/TXT 是否已上传并处理 |
| `ask_case_solver` | 是否已提示用户选择案例 |
| `wait_for_mesh_to_run` | 案例确认完毕，等待上传网格 |
| `uploaded_mesh` | 网格文件是否已上传 |
| `run_completed` | 仿真是否已运行完成 |
| `of_case_zip_generated` | 结果 ZIP 是否已生成 |
| `case_zip_bytes` / `case_zip_filename` | ZIP 文件的内存字节与文件名 |

**侧边栏功能模块（4个）：**
1. **导出聊天记录**：将对话历史序列化为 JSON 下载
2. **上传文档**：上传 PDF/TXT → 提取文本 → 调用 R1 列举案例 → 追加到聊天
3. **上传网格**：上传 `.msh` → 解析边界名称 → 触发仿真流程
4. **下载 OF 案例**：仿真完成后，将 `run_chatcfd/` 下的案例目录打包为 ZIP 供下载

**主聊天区域逻辑（状态机）：**

```
初始 → 文档上传 → R1 列举案例
     ↓
用户输入选择 → R1 解析为结构化 JSON → 展示 Markdown 确认
     ↓
用户可继续修改配置（重复上一步）
     ↓
上传网格 → 自动提取边界 → 调用 main_run_chatcfd.run_case()
     ↓
仿真完成 → 展示运行结果 → 调用 file_summary 生成技术摘要
```

**聊天显示过滤机制：**  
系统内部 prompt（如 `EXTRACT_CASES_FROM_TEXT_PROMPT`、`CONVERT_JSON_TO_MD`）不直接渲染给用户，只展示处理后的结果，保持界面简洁。

### 3.4 CLI 模式：`main2()` 和 `__main__` 块

通过 `argparse` 接收三个参数：
```bash
python src/chatbot.py --document_path <PDF路径> --grid_path <网格路径> --run_time <次数>
```

`main2()` 实现了无前端的完整流程：读取 TXT → 提取案例 → 自动选择 Case_1 → 运行仿真。`manual_verification` 标志可切换为交互式手动选择。

---

## 四、`qa_modules.py` — LLM 调用封装层

**文件大小：** ~316 行  
**核心职责：** 统一封装 DeepSeek V3/R1 的 API 调用、Token 统计、日志写入

### 4.1 `GlobalLogManager`（单例）

```python
class GlobalLogManager:
    current_session_stats = {
        "deepseek-v3": {"calls": 0, "prompt_tokens": 0, "response_tokens": 0},
        "deepseek-r1": {"calls": 0, "prompt_tokens": 0, "response_tokens": 0, "reasoning_tokens": 0}
    }
```

每次 LLM 调用结束后，`GlobalLogManager.add_log()` 会：
1. 累加 `current_session_stats` 中的调用次数和 Token 消耗
2. 若 `config.case_log_write == True`，将完整问答记录追加写入 `{OUTPUT_PATH}/qa_logs.jsonl`

日志采用 JSONL（JSON Lines）格式，每行一条记录，避免内存积累。

### 4.2 V3 系列（生成型任务）

```
BaseQA_deepseek_V3
    ├── QA_Context_deepseek_V3   # 带对话历史，维护 conversation_history
    └── QA_NoContext_deepseek_V3 # 无上下文，每次独立请求（最常用）
```

**底层实现：**  
使用 `OpenAI` SDK 非流式调用（`stream=False`），支持 `response_format` 参数（LangChain Pydantic Parser 格式化输出）。  
失败时自动重试 `retry_times`（默认 3）次。

### 4.3 R1 系列（推理型任务）

```
BaseQA_deepseek_R1
    ├── QA_Context_deepseek_R1   # 带对话历史（用于 RAG 多轮问答）
    └── QA_NoContext_deepseek_R1 # 无上下文，单轮推理（最常用）
```

**R1 的特殊处理：**
- **流式调用（无结构化输出时）**：使用 `stream=True` 逐块读取，同时捕获 `reasoning_content`（`<think>` 过程）
- **非流式调用（有结构化输出时）**：使用 `stream=False`，通过 Pydantic Parser 解析 JSON 响应
- Token 估算：由于流式调用无法获取精确 Token 数，使用 `tiktoken` 对 prompt 和 response 分别估算
- `reasoning_tokens` 单独统计，反映 R1 的思考链长度（对应额外成本）

### 4.4 使用约定

| 任务类型 | 推荐使用 | 说明 |
|---|---|---|
| 文件生成（controlDict、fvSchemes 等） | `QA_NoContext_deepseek_V3` | 生成型，成本低 |
| 错误分析、参数理解、反思 | `QA_NoContext_deepseek_R1` | 推理型，含 `<think>` |
| PDF 语义问答（多轮） | `QA_Context_deepseek_R1` | 带上下文记忆 |
| Web 界面对话 | `ChatBot`（封装了 R1） | 带完整对话历史 |

---

## 五、`pdf_chunk_ask_question.py` — PDF 语义检索（RAG）

**文件大小：** ~194 行  
**核心职责：** 构建 PDF 的 FAISS 向量索引，按语义相似度检索相关段落，传给 R1 回答 CFD 参数问题

### 5.1 `CFDCaseExtractor` 类

#### 初始化
```python
self.embedder = SentenceTransformer("all-mpnet-base-v2")  # 向量嵌入模型
self.index = None    # FAISS 索引
self.chunks = []     # 文本块列表
```
若本地路径不存在，自动 fallback 到在线下载 `sentence-transformers/all-mpnet-base-v2`。

#### `process_pdf(file_path)` — 索引构建

**PDF 处理流程：**
1. `pdfplumber` 逐页提取文本，设置边距 bbox（各 50pt），过滤页眉页脚区域
2. 开启 `layout=True` 布局感知提取，保留文本位置关系
3. `clean_text()` 多阶段清洗：
   - 连接因换行断开的单词（`word-\nend` → `wordend`）
   - 修复数字与单位之间的换行
   - 移除孤立的页码行
   - 压缩多余空行和空格
   - 过滤 3 字符以下的短行
4. `RecursiveCharacterTextSplitter` 分块（chunk_size=600，overlap=100），按章节标题优先分割
5. `SentenceTransformer.encode()` 生成 float32 向量嵌入
6. `faiss.IndexFlatL2` 构建 L2 距离的平坦索引

**TXT 文件处理：** 直接整体作为一个 chunk，不分块（`self.chunks = [text.strip()]`）。

#### `query_case_setup(question, detailed_question, top_k=3, context=False)` — 语义问答

```
用户问题
    ↓ embedder.encode() 向量化
    ↓ faiss.index.search() 检索 top_k 最近邻
    ↓ 过滤：距离 < config.pdf_chunk_d (1.5)，否则使用 top_k 结果
    ↓ 构造 prompt：将检索到的段落 + 问题传给 R1
    ↓ R1 推理并返回答案
```

`context=True` 时使用 `QA_Context_deepseek_R1`（保持多轮对话上下文），`context=False` 时使用无上下文版本。

**关键参数：** `pdf_chunk_d=1.5`，即 L2 距离超过 1.5 的文本块认为与问题不相关，不纳入上下文。

---

## 六、`file_preparation.py` — OpenFOAM 文件生成器

**文件大小：** ~819 行  
**核心职责：** 从 PDF 提取边界条件参数，生成所有 OpenFOAM 配置文件

### 6.1 `extract_boundary_names(grid_file_path, grid_type)` — 网格边界解析

支持两种网格格式：

**`msh` 格式（Fluent 格式）：**  
从文件末尾 `(0 "Zone Sections")` 标记后逐行解析，使用正则 `\(\d+\s+\(\d+\s+\S+\s+(\S+)\)\(\)\)` 提取边界名，过滤 `*_FLUID`、`*_SOLID` 区域，返回纯边界名列表。

**`polyMesh` 格式（OF 原生）：**  
优先用 `PyFoam.ParsedParameterFile` 解析 `boundary` 文件，提取边界名和几何类型的字典。若失败（BinaryList 格式），则 fallback 到正则解析。返回 `{边界名: 几何类型}` 字典。

### 6.2 `case_required_files(solver, turbulence_model, other_physical_model)` — 文件清单确定

**三级搜索策略（严格 → 宽松）：**

```
第1级：solver + turbulence_model + other_physical_model 全部匹配
    ↓ 未找到
第2级：只匹配 solver + turbulence_model
    ↓ 未找到
第3级：只匹配 solver，或同类型（compressible/incompressible）
```

找到候选案例后，**湍流模型文件差异补偿：**  
若参考案例的湍流模型与目标不同，读取 `final_OF_turbulence_required_files.json`，计算：
- `excessive_file`（参考案例有但目标模型不需要的文件，删除）
- `missing_file`（目标模型需要但参考案例没有的文件，补充）

最终调用 V3 模型，从候选案例列表中选出最适合当前仿真需求的文件结构，返回 `(文件集合, 参考案例名, 湍流模型映射)`。

### 6.3 `generate_initial_files(case_description, ...)` — 核心文件生成函数

**流程（约 320 行，是系统最复杂的函数之一）：**

**第一阶段：前置准备**
1. 生成 `system/controlDict` 框架（仅含 `application` 和占位符时间参数）
2. 调用 `convert_mesh()` 转换网格格式
3. 提取转换后的网格边界名和几何类型

**第二阶段：RAG 提取边界条件**
4. 构建 `CFDCaseExtractor` 并处理 PDF
5. **第一次 RAG 问答**：询问边界条件类型 → 得到 `{字段: {边界名: BC类型}}` 的 JSON
6. 根据 `OF_bc_entry.json` 补全各 BC 类型所需的子字段（如 `fixedValue` 需要 `value`，`turbulentIntensityKineticEnergyInlet` 需要 `intensity` 等）
7. **第二次 RAG 问答**：询问各字段的初始值和边界值 → 得到完整的 `{字段: {internalField: ..., boundaryField: {...}}}` JSON

**第三阶段：LLM 生成文件内容**
8. 从知识库加载参考案例文件内容（`processed_merged_OF_cases.json`），按 `0/`、`constant/`、`system/` 分类
9. 调用 `setup_cfl_control()` 设置 CFL 参数（若有参考 `controlDict` 则用 V3 推断 `deltaT`，否则使用默认值）
10. **第三次 RAG 问答（生成 `0/` 文件）**：传入 RAG 上下文 + 边界条件 + 参考文件 → 生成所有初始场文件内容
11. **第四次 RAG 问答（生成 `constant/` 和 `system/` 文件）**：传入参考文件 → 生成物性参数和求解器配置

### 6.4 `check_file_format(files_content)` — 格式合规检查

对每个 `constant/` 和 `system/` 文件调用 V3 模型，与知识库参考文件对比检查格式问题（关键字是否齐全、嵌套结构是否正确等）。

同时执行**维度自动纠正**：根据 `OF_case_dimensions.json` 中预定义的量纲，用正则替换文件中的 `dimensions` 行。特殊处理可压/不可压切换的字段（如压力 `p`：不可压 `[0 2 -2 0 0 0 0]`，可压 `[1 -1 -2 0 0 0 0]`）。

### 6.5 `setup_cfl_control(case_path, max_co, controlDict_ref)` — CFL 参数设置

两种策略：
- **有参考 `controlDict` 且非可压高速求解器**：调用 V3 根据案例描述推断合理的 `deltaT`
- **无参考或可压缩求解器（`rhoCentralFoam`、`sonicFoam`）**：使用硬编码规则（稳态：`endTime=10, deltaT=1`；可压缩瞬态：`deltaT=1e-8`；其他瞬态：`deltaT=1e-5`）

---

## 七、`main_run_chatcfd.py` — 主调度与仿真循环

**文件大小：** ~236 行  
**核心职责：** 协调文件生成、仿真执行、错误修正的完整自动化循环

### 7.1 `load_OF_data_json()` — 按求解器过滤知识库

将 `processed_merged_OF_cases.json`（14MB）加载后，仅保留 `case_path` 中包含当前求解器名称的案例，大幅减少后续检索的数据量：

```python
for case_path, case_info in full_data.items():
    if config.case_info.case_solver in case_path:
        config.OF_case_data_dict[case_path] = case_info
```

### 7.2 `main(case_name_idx)` — 单次案例完整流程

**阶段 1：文件生成**
```
读取 PDF/TXT
    → case_required_files()   确定需要哪些文件
    → generate_initial_files() 生成所有文件内容
    → check_file_format()      格式检查
    → write_field_to_file()    写出磁盘
```

**阶段 2：迭代调试循环（最多 `max_running_test_round=30` 轮）**

```
run_of_case.case_run()
    ├── 成功 → 写入 cycle_index.txt → return "Success"
    └── 失败 → 进入错误修正流程：
            │
            ├── 判断是否需要新增文件
            │   (identify_error_to_add_new_file)
            │       ├── 需要 → add_new_file()
            │       └── 不需要 → analyse_error() → correct_error()
            │
            ├── 检查相同错误是否连续出现
            │   ├── 连续 2 次 → 触发 Reflextion.reflextion()，在下一轮修正中注入反思
            │   └── 连续 4 次 → 放弃修改，整体 rewrite_file()，重置 error_history
            │
            └── 如果 mesh 未转换 → convert_mesh()
                如果 controlDict 时间未设置 → setup_cfl_control()
```

**错误历史管理：**  
`config.error_history` 最多保留最近 4 条，`config.correct_trajectory` 最多保留最近 4 次修正轨迹，防止内存无限增长。

### 7.3 `run_case()` — 多次运行入口

根据 `config.run_cfg.run_time` 重复调用 `main()`，每次使用不同的案例目录名（`{case_name}_0`、`{case_name}_1` 等），用于统计成功率。

---

## 八、`file_corrector.py` — 自动错误修正引擎

**文件大小：** ~1150 行（最大的源文件）  
**核心职责：** 分析 OpenFOAM 运行错误，定位出错文件，生成修正方案并执行

### 8.1 工具函数

| 函数 | 说明 |
|---|---|
| `write_field_to_file(content, path)` | 将字符串写入磁盘，处理 `\n` 转义，自动创建目录 |
| `extract_content_from_response(response, type)` | 从 LLM 回复中提取 JSON 或纯文本（去掉 ` ```json ` 等标记）|
| `extract_pure_response(text)` | 提取 "Here is my response:" 后的内容 |
| `robust_llm_parse(qa, prompt, parser, retries)` | 带重试的 LLM 结构化解析，失败返回 `None` |
| `select_random_items(dict, n)` | 从字典中随机抽取 n 个值长度 ≤ 10000 的条目 |
| `create_OF_case_json(dir)` | 将案例目录的所有文件读入 JSON 字符串 |
| `list_case_file(path)` | 列出案例目录下 `0/`、`system/`、`constant/` 中的所有文件 |
| `clean_json_string(s)` | 修复 LLM 输出中常见的 JSON 转义字符问题 |

### 8.2 错误分析流程

#### `identify_error_to_add_new_file(running_error)` — 缺失文件检测

检查错误信息是否包含 `"cannot find file"`，若有则提取文件路径（格式化为 `0/xx`、`system/xx`、`constant/xx`），否则返回 `"no"`。使用 V3 模型（简单判断，无需深度推理）。

#### `analyse_error(running_error, case_files)` — 两阶段错误定位

**第一阶段（V3）：** 广撒网，从文件列表中找出所有可能有问题的文件，附带错误原因推测。内置 4 个 few-shot 示例（`hFinal` 缺失、质量分数之和为零、浮点异常、`specie` 字段位置错误）。

**第二阶段（R1，仅当可疑文件 > 1 时触发）：** 读取可疑文件的实际内容，精确定位根本原因，缩减到真正出错的文件。同样内置 4 个 few-shot 示例。

#### `analyze_running_error_with_all_case_file_content(running_error)` — 全文件分析

将案例所有文件内容转为 JSON 字符串，一次性传给 R1 分析，直接返回 `{wrong_file, advices_for_revision}` 的 JSON（用于错误重复时的 fallback 路径）。

### 8.3 参考文件检索

#### `find_reference_files(target_file)` — 通用参考文件检索

三级搜索（同 `case_required_files`：全匹配 → 只匹配求解器 → 同类型域），返回 2 个最相关的参考案例文件内容（V3 从候选中选择）。若候选数 > 4，先清空内容、只保留名称，让 V3 选出 2 个名称后再填回内容（节省 Token）。

#### `find_reference_files_by_solver(target_file)` — 面向修正的快速检索

不经过 LLM 筛选，直接从知识库中匹配 solver + turbulence_model，随机取 3 个参考文件（适用于紧急修正场景）。

### 8.4 文件修正流程

#### `correct_error(running_error, error_files, ...)` — 主修正函数

**三步流程：**

1. **查找跨文件依赖（V3）：** 对每个出错文件，确定修正时需要参考哪些其他文件（如修正 `0/CH4` 时需要参考 `0/CO2` 确保质量分数之和为 1）
2. **生成修正建议（R1）：** 结合错误信息、文件内容、相关文件、知识库参考，生成具体逐步修正方案（如"将第 15 行的 relaxationFactor 从 1.5 改为 0.7"）
3. **执行修正（V3）：** 按照修正建议重写文件，并写回磁盘；记录 `{文件名: [原始内容, 修改后内容]}` 到 `correct_trajectory`

#### `single_file_corrector2(file_name, advice, ref_files)` — 单文件修正

直接将修正建议和参考文件传给 V3，输出完整修正后的文件内容（用于 `analyze_running_error_with_all_case_file_content` 返回结果后的执行阶段）。

#### `rewrite_file(file_name, ref_files)` — 文件整体重写

当同一文件连续修正失败 4 次时，放弃修改策略，直接让 V3 参考知识库从头重写整个文件。

#### `add_new_file(file_name)` — 动态新增缺失文件

读取当前案例所有文件内容，让 R1 生成新文件内容（同时执行维度自动纠正，逻辑与 `check_file_format` 中相同）。

---

## 九、`Reflextion.py` — 元认知反思机制

**文件大小：** ~116 行  
**核心职责：** 当相同错误连续出现时，让 R1 反思失败原因，生成下一轮修正的指导建议

### 9.1 `reflextion(running_error, correct_trajectory)` — 触发反思

**触发条件（在 `main_run_chatcfd.py` 中判断）：** 同一错误连续出现 2 次且未达到 4 次阈值。

**反思 Prompt 结构：**

```
当运行 {case_name} 时出现以下错误：
<error_information>{running_error}</error_information>

你已尝试过以下修改：
<file_modifications>
  trial_0: 文件 A 的原始内容 → 修改后内容
  trial_1: 文件 B 的原始内容 → 修改后内容
  ...
</file_modifications>

但错误仍然存在。请分析：
1. 已尝试了哪些方向
2. 遗漏了什么
3. 下一步应该检查什么

请在 "Reflection:" 后给出反思内容。
```

**两个内置 few-shot 示例：**
- 示例1：`counterFlowFlame2D` 中"File 'reactions' does not exist"—— 忽略了 `thermophysicalProperties` 中的文件引用行
- 示例2：`BuoyantCavity` 中"Initial temperature T0 is negative"—— 忽略了 `p_rgh` 的边界条件

### 9.2 `construct_reflection_context(running_error, reflection_history)` — 反思注入

从全局 `reflection_history` 中检索与当前错误相关的历史反思（同一错误优先，不足 3 条则用其他错误的反思补全），构造注入到后续修正 prompt 中的 `<reflection>...</reflection>` 上下文块。

---

## 十、`run_of_case.py` — OpenFOAM 仿真执行器

**文件大小：** ~144 行  
**核心职责：** 通过 `subprocess` 调用 OpenFOAM 求解器，管理 CFL 控制参数

### 10.1 `case_run(case_path)` — 仿真执行

**执行流程：**
1. 直接读取 `system/controlDict` 文本，用字符串查找（而非 PyFoam 解析）提取 `application` 字段的求解器名称（避免 PyFoam 解析失败的问题）
2. 若 `system/setFieldsDict` 在文件结构中，先备份 `0/` 目录（`0.bak`），再执行 `setFields` 初始化场
3. 执行命令：`{solver} -case {case_path} > {case_path}/case_run.log`
4. 通过 `subprocess.run()` 的 `returncode` 判断成功与否：
   - `returncode == 0` → 设置 `config.flag_case_success_run = True`，返回 `"case run success."`
   - `returncode != 0` → 返回 `stderr` 字符串（供错误分析使用）

**注意：** 命令必须使用 `executable="/usr/bin/bash"` 并通过 `shell=True` 运行，确保 OpenFOAM 环境变量生效。

### 10.2 `setup_cfl_control(case_path, max_co=0.6)` — CFL 设置（简化版）

此文件中保留了一个较简单的版本（相比 `file_preparation.py` 中的版本功能更基础），主要处理未能在初始生成阶段完成 CFL 设置时的 fallback 场景。

---

## 十一、`file_summary.py` — 案例摘要生成器

**文件大小：** ~124 行  
**核心职责：** 仿真完成后，生成一份人类可读的技术摘要供用户查看

### 11.1 `get_case_content(case_dir, case_name)` — 读取案例文件

遍历 `run_chatcfd/` 目录下所有匹配 `case_name` 的子目录，读取 `system/`、`constant/`、`0/`（或 `0.bak/`）下的所有文件内容。对超过 9830 字符的文件（通常是场数据文件），截断为前 2000 字符，防止 prompt 过长。通过 `cycle_index.txt` 的存在与否判断案例运行是否成功。

### 11.2 `generate_case_summary(case_description, case_content)` — 生成摘要

调用 `QA_NoContext_deepseek_R1` 生成技术摘要，要求涵盖：
- 物理背景与目标
- OpenFOAM 基本设置（求解器、流态）
- 各文件的关键配置（初始/边界条件、物性参数、数值格式）
- 特殊设置（多相流、化学反应、动网格等）

摘要在 `chatbot.py` 的 Web 界面中，于仿真运行完成后自动展示给用户。

---

## 十二、`preprocess_OF_tutorial.py` — 知识库构建工具（离线）

**文件大小：** ~393 行  
**核心职责：** 离线扫描 OpenFOAM 官方教程目录，构建 `processed_merged_OF_cases.json` 知识库

### 12.1 `case_config_collector()` — 教程案例收集

遍历 `solver_features`（`basic`、`compressible`、`incompressible`、`combustion` 等 8 类），对每个以 `Foam` 结尾的求解器目录：
- 识别包含 `system/controlDict`、`fvSchemes`、`fvSolution` 的合法 OpenFOAM 案例
- 收集 `0/`、`0.orig/`、`constant/`（跳过 `polyMesh`）、`system/` 下所有文件的内容
- 跳过 `blockMeshDict`、`changeDictionaryDict`（与案例配置无关）
- 从 `FoamFile` 行开始截取，去除 OpenFOAM 的文件头注释

### 12.2 `add_case_path_keys(data)` — 元数据提取与标注

对每个收集到的案例提取：
- **`solver`**：从 `system/controlDict` 中用正则提取 `application` 字段
- **`turbulence_type`**：从 `constant/turbulenceProperties` 提取 `simulationType`（RAS/LES/laminar）
- **`turbulence_model`**：从 `turbulenceProperties` 提取 `RASModel` 或 `LESModel`
- **`singlePhase`**：根据求解器是否在 `multiphase_flow_solvers` 列表中判断
- **`particle_flow`**：根据求解器或文件名中是否含 `Cloud` 判断
- **`reacting_flow`**：根据求解器或 `combustionProperties`、`reactions` 文件存在与否判断
- **`boundary_type`**：从所有 `0/` 文件中提取 `boundaryField` 中出现的所有 BC 类型
- **`0.orig/` → `0/` 路径归一化**：将 `0.orig/field.orig` 重命名为 `0/field`

### 12.3 `main()` — 完整构建流程

```
case_config_collector()              # 原始数据收集 → openfoam_cases.json
    ↓ describe_cases()               # 过滤超长案例（> 200KB）
    ↓ merge_json_objects()           # 合并多个 JSON 对象
    ↓ add_case_path_keys()           # 提取元数据
    → processed_merged_OF_cases.json # 最终知识库（14MB）
```

### 12.4 `read_in_processed_merged_OF_cases()` — 运行时加载

在 `chatbot.py`（上传网格后触发仿真前）调用，将知识库读入 `config.global_OF_cases` 并重建 `solver_set`、`turbulence_model_set` 等关键字集合。

---

## 十三、`case_file_requirements.py` — 网格边界解析（旧版）

**文件大小：** ~45 行  
**核心职责：** 从 Fluent `.msh` 文件中解析边界名称（早期实现，功能已被 `file_preparation.py` 中的 `extract_boundary_names` 覆盖）

仅支持 `msh` 格式，使用与 `file_preparation.py` 中 `msh` 分支相同的解析逻辑（搜索 `Zone Sections` 标记，正则匹配 `(39 ...)` 行），但将结果写入 `config.case_boundaries`（全局变量）而非返回值。

**状态：** 此文件在当前代码流程中已不再被主流程调用，可能是历史遗留文件。

---

## 附：各文件代码量与使用 LLM 模型汇总

| 文件 | 行数 | 使用 V3 | 使用 R1 | 说明 |
|---|---|---|---|---|
| `config.py` | ~474 | — | — | 配置和常量，无 LLM 调用 |
| `chatbot.py` | ~808 | — | ✅ | Web 对话、案例选择 |
| `qa_modules.py` | ~316 | ✅ | ✅ | LLM 调用底层封装 |
| `pdf_chunk_ask_question.py` | ~194 | — | ✅ | RAG 检索问答 |
| `file_preparation.py` | ~819 | ✅ | ✅ | R1 用于 RAG，V3 用于格式检查 |
| `main_run_chatcfd.py` | ~236 | — | — | 纯调度逻辑 |
| `file_corrector.py` | ~1150 | ✅ | ✅ | R1 分析/反思，V3 生成修正 |
| `Reflextion.py` | ~116 | — | ✅ | 反思推理 |
| `run_of_case.py` | ~144 | — | — | 纯系统调用 |
| `file_summary.py` | ~124 | — | ✅ | 摘要生成 |
| `preprocess_OF_tutorial.py` | ~393 | — | — | 离线工具，无 LLM |
| `case_file_requirements.py` | ~45 | — | — | 旧版工具，无 LLM |
