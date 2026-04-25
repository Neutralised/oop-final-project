# OOP Final Project

北京理工大学大二下学期 面向对象技术与方法 结课项目。

仓库地址：[https://github.com/Neutralised/oop-final-project](https://github.com/Neutralised/oop-final-project)。

项目报告见目录下的 report.pdf。

## 使用

### 编译并运行

在项目目录下：

```bash
g++ -o main -std=c++17 airline.cpp solver.cpp main.cpp
```

随后启动 `main.exe`，

```bash
./main
```

> [!NOTE]
>
> 由于 `solver.cpp` 中包含 structured bindings 等 C++17 特性，编译时如不加入 `-std=c++17` 会出现警告。

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