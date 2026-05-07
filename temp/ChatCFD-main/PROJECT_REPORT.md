# ChatCFD 项目详细分析报告

## 一、项目概述

**ChatCFD** 是一款 **LLM 驱动的端到端计算流体力学（CFD）自动化智能体系统**。其核心目标是通过大语言模型降低 CFD 仿真的使用门槛：用户只需用自然语言描述仿真需求，或直接上传学术论文 PDF，系统即可自动完成 OpenFOAM 案例的全套配置并驱动仿真运行。

论文已发表于期刊 *Advanced Intelligent Discovery*（2026），并在 arXiv（2506.02019）公开预印本。

---

## 二、核心用途

| 场景 | 说明 |
|---|---|
| **论文复现** | 上传 CFD 学术论文 PDF，系统自动提取仿真参数并配置 OpenFOAM 案例 |
| **自然语言驱动仿真** | 用一句话描述仿真需求（如"NACA0012 翼型，攻角 10 度，k-ω SST 湍流模型"），自动生成完整仿真案例 |
| **错误自愈** | 仿真运行失败时，系统自动分析错误并迭代修正配置文件，最多尝试 30 轮 |
| **批量处理** | 通过命令行模式批量运行多个案例 |

**性能指标（论文数据）：**

- 315 个 OpenFOAM 基础案例：运行成功率 **82%**，物理保真度 **59%**，平均单案例成本 **$0.20**
- 学术论文复现（NACA0012 不可压）：成功率 **40%**
- 学术论文复现（可压缩喷嘴）：成功率 **30%**

---

## 三、技术栈

### 3.1 AI / 大语言模型层

| 组件 | 版本/说明 |
|---|---|
| **DeepSeek-R1** | 推理模型，用于复杂的 CFD 参数理解、反思纠错（temperature=0.9） |
| **DeepSeek-V3** | 对话/生成模型，用于文件生成、修正（temperature=0.7） |
| **Sentence-Transformers** | `all-mpnet-base-v2`，向量嵌入模型，用于 PDF 语义检索 |
| **FAISS** | Facebook AI 向量相似度搜索库（faiss-cpu 1.7.4） |
| **LangChain** | 结构化输出解析（Pydantic OutputParser） |
| **tiktoken** | OpenAI token 计数工具（成本统计） |

### 3.2 CFD 仿真层

| 组件 | 版本/说明 |
|---|---|
| **OpenFOAM** | v2406，开源 CFD 框架，实际的仿真求解器 |
| **PyFoam** | 2023.7，Python 接口，用于读写 OpenFOAM 配置文件 |
| **fluentMeshToFoam** | OpenFOAM 内置工具，将 Fluent `.msh` 格式网格转换为 OF 格式 |

### 3.3 PDF 处理层

| 组件 | 版本/说明 |
|---|---|
| **pdfplumber** | 0.11.5，带布局感知的 PDF 文本/表格提取 |
| **PyPDF2** | 3.0.1，PDF 读取辅助 |
| **pdfminer.six** | PDF 底层解析 |

### 3.4 前端与 API 层

| 组件 | 版本/说明 |
|---|---|
| **Streamlit** | 1.41.1，Web 聊天界面框架 |
| **OpenAI Python SDK** | 1.39.0，兼容 OpenAI 接口格式，调用 DeepSeek API |
| **PyTorch** | 2.6.0，为 Sentence-Transformer 提供底层推理支持 |
| **FastAPI** | Web API 支持（可选） |

### 3.5 其他

- **Python** 3.11.4
- **操作系统**：Linux / Windows WSL2（OpenFOAM 需要 Linux 环境）
- **conda** 环境管理（`chatcfd_env.yml`）

---

## 四、如何运行

### 前置条件

1. Linux 或 Windows WSL2 环境
2. 已安装 OpenFOAM v2406
3. 已配置 conda 环境
4. 拥有 DeepSeek API Key（V3 + R1 各一个）

### 安装步骤

```bash
# 1. 克隆仓库
git clone https://github.com/ConMoo/ChatCFD.git
cd ChatCFD

# 2. 创建 conda 环境
conda env create -f chatcfd_env.yml
conda activate chatcfd

# 3. 下载 Sentence Transformer 模型
python test_env/download_model.py

# 4. 安装 OpenFOAM v2406
curl -s https://dl.openfoam.com/add-debian-repo.sh | sudo bash
sudo apt-get install openfoam2406-default
```

### 配置文件 `inputs/chatcfd_config.json`

```json
{
    "DEEPSEEK_V3_KEY": "你的API密钥",
    "DEEPSEEK_V3_BASE_URL": "API端点URL",
    "DEEPSEEK_V3_MODEL_NAME": "deepseek-v3-250324",
    "V3_temperature": 0.7,
    "DEEPSEEK_R1_KEY": "你的API密钥",
    "DEEPSEEK_R1_BASE_URL": "API端点URL",
    "DEEPSEEK_R1_MODEL_NAME": "deepseek-r1-250528",
    "R1_temperature": 0.9,
    "run_time": 1,
    "OpenFOAM_path": "/usr/lib/openfoam/openfoam2406",
    "OpenFOAM_tutorial_path": "/usr/lib/openfoam/openfoam2406/tutorials",
    "max_running_test_round": 30,
    "pdf_chunk_d": 1.5,
    "sentence_transformer_path": ""
}
```

### 运行方式

**方式一：Web 交互界面（推荐）**

```bash
streamlit run src/chatbot.py --server.port 8501
```

访问 `http://localhost:8501`，按界面提示上传 PDF 文档和网格文件。

操作步骤：
1. 在 `Upload the Documents` 区域上传 PDF 论文或自然语言描述文件
2. ChatCFD 自动解析文档，列出所有可执行的 CFD 案例
3. 选择目标案例，通过对话确认求解器、湍流模型、边界条件等参数
4. 在 `Upload the mesh file` 区域上传对应的网格文件（`.msh` 格式）
5. 系统自动完成配置生成并启动仿真，结果保存至 `run_chatcfd/`

**方式二：命令行批处理**

```bash
python src/chatbot.py \
  --document_path "pdf/sun_2023_naca0012.pdf" \
  --grid_path "grids/naca0012.msh" \
  --run_time 1
```

输出结果保存在 `run_chatcfd/` 目录下。

---

## 五、目录与文件详解

### 根目录

| 文件/目录 | 作用 |
|---|---|
| `chatcfd_env.yml` | conda 完整环境定义文件，含所有依赖包及版本（推荐安装方式） |
| `requirements.txt` | pip 最小依赖列表，备用安装方案 |
| `ReadMe.md` | 项目主文档，含安装、运行、性能指标等完整说明 |
| `LICENSE.txt` | 开源许可证 |

---

### `src/` — 核心源码目录

```
src/
├── chatbot.py                  ← 主入口：Web界面 + CLI入口
├── config.py                   ← 全局配置管理
├── main_run_chatcfd.py         ← 主调度流程
├── file_preparation.py         ← OpenFOAM文件生成
├── file_corrector.py           ← 错误检测与自动修正
├── qa_modules.py               ← LLM调用封装层
├── Reflextion.py               ← 反思机制（R1模型）
├── run_of_case.py              ← OpenFOAM仿真执行
├── pdf_chunk_ask_question.py   ← PDF语义检索（RAG）
├── case_file_requirements.py   ← 网格边界解析
├── file_summary.py             ← 案例文件摘要
└── preprocess_OF_tutorial.py   ← 知识库预处理（离线使用）
```

#### `chatbot.py` — 系统主入口

整个项目最核心的文件，双重身份：
- **Streamlit Web 应用**：实现 ChatGPT 风格的聊天界面，支持 PDF 上传、网格文件上传、多轮对话
- **CLI 入口**：通过 `argparse` 解析 `--document_path`、`--grid_path`、`--run_time` 参数直接运行

主要流程：从文档中提取所有 CFD 案例列表 → 用户选择具体案例 → 确认仿真参数 → 调用 `main_run_chatcfd.run_case()` 执行。

#### `config.py` — 全局配置与状态管理

使用 Python `dataclass` 定义多组配置类：

| 配置类 | 说明 |
|---|---|
| `path_config` | 所有文件路径（输入/输出/数据库/临时目录） |
| `llm_config` | DeepSeek R1 和 V3 的 API Key、URL、temperature |
| `run_config` | 运行模式、最大迭代轮数 |
| `case_info` | 当前案例的求解器、湍流模型、边界条件、文件结构等运行时状态 |
| `grid_info` | 网格文件的边界信息 |

同时包含全局变量（`error_history`、`correct_trajectory` 等），用于跨模块共享仿真状态。

#### `main_run_chatcfd.py` — 核心调度循环

实现整个端到端自动化的主循环，关键步骤：
1. 读取 PDF/TXT 文档提取论文内容
2. 查询求解器所需文件列表（来自 `database_OFv24` 知识库）
3. 调用 `file_preparation` 生成初始 OpenFOAM 配置文件
4. 写出文件到磁盘
5. **迭代调试循环**（最多 `max_running_test_round` 轮）：
   - 调用 `run_of_case.case_run()` 运行仿真
   - 若失败：分析错误 → 判断是修改文件还是新增文件
   - 若相同错误重复出现 → 触发 `Reflextion`（反思）
   - 连续 4 次失败 → 整体重写该文件

#### `file_preparation.py` — OpenFOAM 文件生成器

负责将 LLM 理解的案例参数转化为实际的 OpenFOAM 格式文件，包括：
- 查询 `database_OFv24` 确定所需文件列表
- 调用 LLM（V3）生成 `0/U`、`0/p`、`constant/transportProperties`、`system/controlDict` 等所有配置文件
- 网格格式转换（`fluentMeshToFoam`：`.msh` → OpenFOAM polyMesh）
- 文件格式合规性检查（维度检查等）

#### `file_corrector.py` — 自动错误修正引擎

系统"自愈"能力的核心（1100+ 行），主要功能：

| 函数 | 说明 |
|---|---|
| `analyse_error()` | 让 LLM（R1）分析运行错误，定位出错文件 |
| `correct_error()` | 根据错误信息和参考文件修正配置 |
| `add_new_file()` | 判断是否需要新增缺失文件（如 `reactions`、`setFieldsDict` 等） |
| `rewrite_file()` | 在多次修正失败后整体重写目标文件 |
| `find_reference_files_by_solver()` | 从知识库中检索同类求解器的参考配置 |

#### `qa_modules.py` — LLM 调用封装层

封装了对 DeepSeek V3 和 R1 的 API 调用：
- `QA_NoContext_deepseek_V3`：调用 V3（生成型任务，成本低）
- `QA_NoContext_deepseek_R1`：调用 R1（推理型任务，含 `<think>` 过程）
- `GlobalLogManager`：单例，记录每次 LLM 调用的 Token 消耗（分 prompt/response/reasoning 三类），写入 `qa_logs.jsonl`

#### `Reflextion.py` — 反思机制

当同一错误连续出现时，调用 R1 模型进行"元认知"反思：分析哪些修改方向已尝试过、遗漏了什么、下一步应检查什么，生成结构化的反思结论并注入下一轮修正 prompt，避免在同一错误上循环。

#### `run_of_case.py` — 仿真执行器

通过 Python `subprocess` 直接调用 OpenFOAM 求解器：
- 读取 `system/controlDict` 获取 `application`（求解器名称）
- 若有 `setFieldsDict`，先执行 `setFields` 初始化场
- 执行 `{solver} -case {case_path}` 并捕获日志
- 自动设置 CFL 控制参数（`adjustTimeStep`、`maxCo`、`deltaT` 等）

#### `pdf_chunk_ask_question.py` — PDF 语义检索模块

实现 RAG（检索增强生成）流程：
1. 使用 `pdfplumber` 提取 PDF 文本（带布局感知，过滤页眉页脚）
2. `RecursiveCharacterTextSplitter` 分块
3. `SentenceTransformer(all-mpnet-base-v2)` 生成向量嵌入
4. `FAISS` 建立向量索引，按语义相似度检索最相关文本块
5. 将检索到的上下文传入 R1 模型回答具体的 CFD 参数问题

#### `case_file_requirements.py` — 网格边界解析

从 Fluent `.msh` 网格文件中自动解析边界条件名称（如 `inlet`、`outlet`、`wall` 等），为后续的 OpenFOAM 边界条件配置提供输入。

#### `file_summary.py` — 案例文件摘要

生成当前 OpenFOAM 案例配置文件的摘要描述，用于在聊天界面中向用户展示案例配置概况。

#### `preprocess_OF_tutorial.py` — 知识库构建脚本（离线）

用于构建 `database_OFv24/processed_merged_OF_cases.json`（14MB 的知识库文件）。从 OpenFOAM 官方教程目录中解析所有案例文件，建立求解器 → 文件内容的结构化映射，供运行时检索参考。**通常只需离线运行一次，不影响日常使用。**

---

### `database_OFv24/` — OpenFOAM 知识库

| 文件 | 大小 | 说明 |
|---|---|---|
| `processed_merged_OF_cases.json` | **14 MB** | 最核心的知识库，包含 OpenFOAM v2406 所有官方教程案例的解析内容，按求解器索引 |
| `final_OF_solver_required_files.json` | 40 KB | 每个求解器（如 `simpleFoam`、`rhoCentralFoam`）所需的文件清单 |
| `final_OF_turbulence_required_files.json` | 5 KB | 各湍流模型（k-ε、k-ω SST、Spalart-Allmaras 等）所需的额外文件清单 |
| `OF_bc_entry.json` | 8 KB | OpenFOAM 边界条件词典，各类 BC 的合法字段定义 |
| `OF_case_dimensions.json` | 12 KB | 场变量的量纲信息（如速度 `[m/s]`、压力 `[Pa]`） |

---

### `inputs/` — 运行时配置

| 文件 | 说明 |
|---|---|
| `chatcfd_config.json` | **必须配置**。包含 DeepSeek API Key/URL/模型名、OpenFOAM 安装路径、Sentence Transformer 路径、最大迭代轮数等所有运行参数 |

---

### `grids/` — 示例网格文件

| 文件 | 说明 |
|---|---|
| `naca0012.msh` | NACA0012 翼型的 Fluent 格式网格（~3 MB） |
| `Yu_2023_nozzle.msh` | 喷嘴算例网格（~6 MB） |

---

### `pdf/` — 示例论文

| 文件 | 对应案例 |
|---|---|
| `sun_2023_naca0012.pdf` | 不可压缩 NACA0012 翼型绕流仿真论文 |
| `Yu_2023_nozzle.pdf` | 可压缩喷嘴流动仿真论文 |

---

### `run_chatcfd/` — 仿真输出目录

存放所有 ChatCFD 生成和运行的 OpenFOAM 案例，目录结构与标准 OpenFOAM 案例一致：

```
run_chatcfd/
└── sample_NACA0012_AOA10_kOmegaSST/   ← NACA0012 AOA10° k-ω SST 完整案例示例
    ├── 0/                              ← 初始场（U, p, k, omega, nut, nuTilda）
    ├── 5/                              ← 第5步计算结果（各场变量）
    ├── 10/                             ← 第10步计算结果
    ├── constant/                       ← 物性参数（transportProperties, turbulenceProperties）
    ├── system/                         ← 仿真控制（controlDict, fvSchemes, fvSolution）
    ├── all_qa_logs.json               ← 本次仿真所有LLM调用日志（问答记录+Token消耗）
    └── case_run.log                   ← OpenFOAM求解器运行日志
```

---

### `figures/` — 项目说明图

| 文件 | 内容 |
|---|---|
| `fig1.illustration2.png` | 系统整体功能示意图 |
| `fig2.corescheme.png` | 多智能体架构核心流程图 |
| `chatcfd_framework_4.png` | 系统框架图（详细版） |
| `chatcfd_overview_4.png` | 系统概览图 |
| `web.png` / `user_interface_demo.png` | Web 界面截图 |

---

### `test_env/` — 环境验证脚本

| 文件 | 作用 |
|---|---|
| `download_model.py` | 从 HuggingFace 下载 `all-mpnet-base-v2` 向量嵌入模型到本地 |
| `test_all_mpnet_base_v2.py` | 测试模型是否正确加载并能计算文本相似度（GPU/CPU 环境均支持） |

---

## 六、系统工作流程总结

```
用户输入（PDF论文 / 自然语言）
          │
          ▼
   chatbot.py（Streamlit UI / CLI）
          │  用 R1 从 PDF 中提取 CFD 案例列表
          │  用户确认案例参数
          ▼
   main_run_chatcfd.py（主调度循环）
          │
          ├─► file_preparation.py  ──► LLM(V3) 生成所有 OF 配置文件
          │       │ 查询 database_OFv24 确定文件清单
          │       │ 调用 pdf_chunk_ask_question (RAG) 检索参数
          │       └─► 网格格式转换（fluentMeshToFoam）
          │
          ├─► run_of_case.py  ──► 调用 OpenFOAM 求解器（subprocess）
          │
          └─► [仿真失败？] file_corrector.py
                  │  LLM(R1) 分析错误 → 定位出错文件
                  │  修正/新增文件  ←──────────────────────┐
                  │  同一错误重复 → Reflextion.py 反思 ────┘
                  └─► 最多 30 轮迭代
                          │
                          ▼
                  仿真成功 → 输出到 run_chatcfd/
```

---

## 七、总结

ChatCFD 是一个将 **LLM 推理能力与专业领域知识库深度融合** 的工程系统，主要创新点如下：

| 创新点 | 说明 |
|---|---|
| **双模型协作** | V3（快速生成）+ R1（深度推理）分工明确，降低成本同时保证准确性 |
| **结构化知识库** | 预处理 OpenFOAM 全量教程（14MB），为文件生成提供精确的领域先验 |
| **反思-迭代纠错** | 不是简单重试，而是通过元认知反思避免重复犯同类错误 |
| **RAG 增强参数提取** | FAISS 向量检索确保从长篇论文中准确抓取关键仿真参数 |
