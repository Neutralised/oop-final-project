# OOP Final Project

北京理工大学大二下学期 面向对象技术与方法 结课项目。

仓库地址：[https://github.com/Neutralised/oop-final-project](https://github.com/Neutralised/oop-final-project)。

项目报告见目录下的 report.pdf。

## 项目结构

```sh
.
├── README.md
├── build.bat
├── CMakeLists.txt
├── docs
│   ├── assets        # 报告内含图片目录
│   ├── report.md     # 项目报告
│   └── report.pdf    # 项目报告
├── include
│   ├── log.h         # 交互时输出日志
│   ├── airline.h     # 定义 Airline 类
│   └── solver.h      # 定义 Solver 类
├── sources
│   ├── airline.cpp   # 实现 Airline 类
│   ├── solver.cpp    # 实现 Solver 类
│   └── main.cpp      # 主循环
└── tests
    ├── test1.csv
    └── generator.cpp # 大规模数据生成器
```

## 使用

### 项目依赖

- CMake 3.10 或更高版本

- MinGW-W64 编译器（包含 g++）

### 编译并运行

运行项目目录下的 `build.bat`，看到以下信息：

```bash
=================================
Flight Reservation System Builder
=================================


[1] Configuring CMake with MinGW...
-- The CXX compiler identification is GNU 8.1.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: mingw64路径/bin/c++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.5s)
-- Generating done (0.0s)
-- Build files have been written to: 项目路径/build

[2] Building project with MinGW...
[ 16%] Building CXX object CMakeFiles/main.dir/sources/main.cpp.obj
[ 33%] Building CXX object CMakeFiles/main.dir/sources/solver.cpp.obj
[ 50%] Building CXX object CMakeFiles/main.dir/sources/airline.cpp.obj
[ 66%] Linking CXX executable "项目路径\main.exe"
[ 66%] Built target main
[ 83%] Building CXX object CMakeFiles/generator.dir/tests/generator.cpp.obj
[100%] Linking CXX executable "项目路径\tests\generator.exe"
[100%] Built target generator

=================================
Build completed successfully!
Executable: main.exe
Test data generator: tests/generator.exe
=================================

请按任意键继续. . .
```

说明项目编译成功。

随后启动 `main.exe` 进行自定义测试：

```bash
./main
```

在 `tests` 目录下，有一个已生成好的 CSV 文件 `test1.csv` 用于小规模测试。

另外，可以运行该目录下的 `generate.bat`，以生成一份包含 10000 条航班信息的 `test2.csv` 文件，用于大规模测试。修改 `generator.cpp` 可以更改生成的航班信息数量。

> [!NOTE]
>
> 由于 `solver.cpp` 中包含 structured bindings 等 C++17 特性，如不以 C++17 标准编译，会出现警告。标准可在 `CMakeList.txt` 中进行修改。

### 输入格式说明

- 在主循环中，启动时/输入 `8` 时会打印操作菜单，输入对应数字即可进行操作。

- 在主循环中，输入 `7` 会打印格式帮助，可以在这里找到时间和 CSV 格式要求。

- 向系统中加入航班信息有两种方式：在控制台输入单个航班信息；从本地 CSV 文件中读取多个航班信息。用于输入的 CSV 文件需要满足下述格式要求。

- 输入日期格式：`YYYY-MM-DD`。

- 输入时间格式：`YYYY-MM-DD HH:MM`。

- CSV 文件输入/输出格式：

  输入文件应当至少包含以下列：

  | Flight ID | Departure Station | Terminal Station | Departure Time | Arrival Time | Initial Ticket Count |
  | :-------: | :---------------: | :--------------: | :------------: | :----------: | :------------------: |

  - `Flight ID` 的格式应为 `XXYYYY`，其中 `XX` 为英文字符，`YYYY` 为数字。

  - `Departure/Terminal Station` 无特殊格式限制。
  - `Departure/Arrival Time` 格式见时间格式。
  - `Initial Ticket Count` 为一非负整数，表示航班初始票容量。

  此外，可以在末尾添加列 `Reserved Users`，格式为 `user_id:count;user_id:count...`，为各预订了该航班的用户 id 和预订票数。保存航班信息到本地时默认有此列。

### 输出格式说明

- `[INFO]` 表示一般信息。

- `[ERROR]` 信息表示出现错误，如航班号格式错误、未查询到航班等。
- `[OP]` 表示用户操作成功。
- `[FLIGHT]` 表示一个航班相关的信息。
- `[NOTICE]` 表示提示信息。