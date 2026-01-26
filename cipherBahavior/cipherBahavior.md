# GUI WORKFLOW
## 1.Fluent


## 2.cipher
 #### 1. Prepare：在文件夹下 在文件夹下放入cgns 或者 cas文件 以及 STEP1.yaml，然后编辑修改STEP1.yaml
 #### 2. step1：生成用于step2的STEP2.yaml文件，同时定义step2 boundary分类的预规则
 - 用户可以编辑一开始准备的STEP1.yaml，yaml内部的格式如[STEP1.YAML](./STEP-1-Config.yaml) 所示。通过修改内部内容来控制预先设置的匹配规则
 
 #### 3. step2: 为了生成适应于solver自己的输入文件的格式，根据step2.yaml内部的boundary分类规则,会对实现准备的cas 或者cgns 文件进行初步的转换，生成zjui.pre.h5文件，同时生成用于step3 的 STEP3.yaml文件。
 - 用户对STEP2.yaml中的内部进行修改，yaml内部的格式如[STEP2.YAML](./STEP-2-Convert.yaml) 所示。yaml内部各部分的boundary会根据step1中设定的预规则在这里已经进行初步分类，用户可能需要对部分未正确分类或者ungroup的boundary进行修改。
 - 此外STEP2中也存在部分其他关于solver的一些setting选项，step3会根据这里的设定来生成更加细致的选项。如果step2算是选项，那么step3就是分选项。
 #### 4. step3: 设定所有solver所需要的设定选项，并且生成solver真正需要的设定文件zjui.cfg.同时将zjui.pre.h5进一步转化成适配于solver的 zjui.grid.hdf文件，同时生成solver所需要的初场文件zjui.flow.hdf文件。（在中途会生成临时文件：zjui.zone.h5）
 - 用户根据需要设置yaml里面的选项，内部如[STEP3.YAML](./STEP-3-Setup.yaml) 所示。
 #### 4. solver run:需要的输入文件 zjui.cfg , zjui.flow.hdf ,zjui.grid.hdf. 
 
 在文件夹下放入cgns 或者 cas文件 以及 STEP1.yaml，然后编辑修改STEP1.yaml(用于定义)
 -
1. 准备好网格文件（**cas.h5** 或者 **cgns**)，生成 **STEP-1-Config.yaml**（GUI）/ 创建编辑 **STEP-1-Config.yaml**（TUI）
2. 调用 ```cipher-pre step1```，将原始网格文件转换为中间网格文件 （**aes.h5**），根据 **STEP-1-Config.yaml** 生成 **STEP-2-Convert.yaml** 模板文件
3. 编辑并保存**STEP-2-Convert.yaml**
4. 运行 ```cipher-pre step2```， 读取 **STEP-2-Convert.yaml**  和中间网格文件 （**aes.h5**）生成**STEP-3-Convert.yaml**
5. 运行 ```cipher-pre step3```， 读取 **STEP-3-Convert.yaml**  和中间网格文件 （**aes.h5**）生成 ```cipher.cfg``` 文件，首先生成按照 zone 分开存储的 **zone.h5** 文件，随后把 **zone.h5**文件中各zone连接求解器所需网格文件 (**grid.h5**)  和流场文件 (**flow.h5**)
6. 编辑 ```cipher.cfg```  文件并运行求解器 **cipher-solver**
7. 使用```cipher-tools``` 将所有网格和流场文件转换为后处理可用的 **cgns** 文件


## 3.GUI
 - 
 -
 -