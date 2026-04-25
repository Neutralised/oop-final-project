# OOP Final Project Report

## 需求分析

- 存储航班各种信息：

  实现 `Airline` 类，包含成员：

  - `std::string id_`：航班号；
  - `std::string start_`：始发站；
  - `std::string terminal_`：终点站；
  - `system_clock::time_point depart_time_`：起飞时间；
  - `system_clock::time_point arrive_time_`：到达时间；
  - `int ticket_count_`：初始票容量；
  - `int reserved_count_`：剩余票量。

- 从文件中读取航班信息：

  选取 `.csv` 格式的文件进行读写。

- 打印特定航班的详细信息（航班号，始终站，飞行时长，余票数量）：

  在航班类 `Airline` 中实现 `printInfo` 方法。 

- 用户通过输入航班号/起飞时间/终点站查询特定航班：

  实际情况中用户查询特定起飞时间时，一般不会精确到小时，故使用起飞日期来查询。

  由于需要支持通过特定键值查询航班，故使用哈希表，例如 `std::unordered_map` 和 `__gnu_pbds::gp_hash_table`。由于后者效率更高，这里采用后者。

  使用 `std::vector<Airline>` 存储当前所有航班，并用三个 `__gnu_pbds::gp_hash_table` 存储特定航班号/起飞日期/终点站的航班在当前航班序列中的下标（不存储实例以节约空间）。

  由于多个航班的起飞日期/终点站可能相同，故后两者需要存储 `std::string, std::vector<int>` 键值对。在查询时，如有多个可能航班，将向用户打印这些航班并选择其一。

- 用户订票/退订：

  需要支持多个用户，并根据特定航班检查用户是否有预订信息，故需在 `Airline` 类中包含一个 `__gnu_pbds::gp_hash_table` 成员，用于存储“用户 id + 订票数量”键值对。

## 模块设计

### 核心类设计

#### `Airline` 类

封装航班相关的数据和操作。

成员变量：

```cpp
private:
	std::string id_; // 航班号
	std::string start_; // 始发站
	std::string terminal_; // 终到站
	system_clock::time_point depart_time_; // 起飞时间
	system_clock::time_point arrive_time_; // 到达时间
	int ticket_count_; // 初始票数量
	int reserved_count_; // 已预订票数量
	__gnu_pbds::gp_hash_table<std::string, int> reserved_users_; // 当前预订此航班的客户 id 以及数量
```

成员函数：

```cpp
public:
	explicit Airline(std::string id = "$",
		std::string start = "$",
		std::string terminal = "$",
		std::string depart = "1970-01-01 00:00",
		std::string arrive = "1970-01-01 00:00",
		int ticket_count = -1);
	~Airline();
	std::string id() const;
	std::string start() const;
	std::string terminal() const;
	system_clock::time_point depart_time() const;
	system_clock::time_point arrive_time() const;
	int ticket_count() const; // 当前余票
	int initial_ticket_count() const; // 初始总票数
	const __gnu_pbds::gp_hash_table<std::string, int>& reserved_users() const; // 预订用户信息
	minutes calcDuration(); // 计算飞行时长并以分钟返回
	int addReservation(std::string user_id, int count); // 为该航班添加一位客户的预订
	int delReservation(std::string user_id, int count); // 为该航班取消一位客户的预订
	void printTime(const system_clock::time_point& tp); // 打印 timepoint
	void printInfo(); // 打印信息：1.航班号 2.起点/终点 3.起飞/到达时间 4.飞行时长 5.余票
```

#### `Solver` 类

用于实现与用户交互并维护航班信息。

成员变量：

```cpp
private:
	Airline null_flight; // 用于查找失败时返回的默认航班
	std::vector<Airline> flights_; // 航班序列
	__gnu_pbds::gp_hash_table<std::string, int> ht_id_; // 用于按 id 查找航班在航班序列中的下标
	__gnu_pbds::gp_hash_table<std::string, std::vector<int>> ht_depart_; // 按出发时间查找，此处按日期分类，之后会列出本日所有航班供用户选择
	__gnu_pbds::gp_hash_table<std::string, std::vector<int>> ht_terminal_; // 按终点站查找
	// 由于多个航班出发时间和终点站可能相同，需要使用 vector 存储下标序列
```

成员函数：

```cpp
public:
	Solver();
	~Solver();
	void addFlight(); // 通过控制台输入添加一个新航班
	Airline& queryFlight(const std::string& key, int type); // 通过 id/出发时间/终点站 获取航班 
	Airline& queryFlightInteract(); // 用户获取航班
	void reserveTicket(const std::string& user_id, int count, Airline& flight); // 向航班中添加用户订票信息
	void cancelTicket(const std::string& user_id, int count, Airline& flight); // 向航班中添加用户取消订票信息
	void reserveFlight(Airline& flight); // 用户订票（已知航班）
	void cancelFlight(Airline& flight); // 用户取消订票（已知航班）
	void reserveFlight(); // 用户订票（未知航班）
	void cancelFlight(); // 用户取消订票（未知航班）
	void addFlightFromFile(const std::string& path); // 从文件中读取新增航班信息
	void saveFlightToFile(const std::string& path); // 保存航班信息到文件
	void startInteract(); // 开始交互
```

### CSV 文件格式

见 README.md。

### 核心功能

#### 处理时间

采用 C++ `chrono` 库，实现了 `timeToString`、`printTime` 等函数/方法，用于字符串和时间的类型转换。

#### CSV 文件读写

实现了以下函数用于用户通过文件输入航班信息/向文件中存储当前航班信息（包括预订）：

```cpp
void addFlightFromFile(const std::string& path);
void saveFlightToFile(const std::string& path);
```

#### 哈希表实现快速操作

采用 GNU pbds 库中的高效哈希表 `gp_hash_table`，以实现以下操作时间复杂度：

- 按 ID 查询航班：$O(1)$；
- 按起飞日期/终点站查询航班：$O(k)$，其中 $k$ 为同一起飞日期/终点站航班数；
- 为某一用户预订/退订特定航班的若干张机票：$O(1)$。

## 程序测试分析

### 测试用例1（目录下的 `test1.csv`）

```
Flight ID,Departure Station,Terminal Station,Departure Time,Arrival Time,Initial Ticket Count
FK7891,Beijing,Ningbo,2026-04-23 14:13,2026-04-23 16:23,180,
FK9178,Ningbo,Beijing,2026-04-23 22:26,2026-04-23 23:55,220,
```

用于测试基本功能：根据三种关键字查询航班、预订/退订航班、保存至本地 CSV 文件。

#### 流程测试

![test1-1](.\assets\test1-1.png)

启动 `main.exe` 后，正常打印操作菜单。

![test1-2](.\assets\test1-2.png)

输入 `2` 从 `tests/test1.csv` 中读取了两个航班信息，并根据航班号对 FK7891 航班进行了查询。查询成功后，系统询问用户是否要展示该趟航班信息、是否要预订或取消预订。可以看到，`Airline` 类正确地根据起飞时间和到达时间计算了航班的飞行时长。

![test1-3](.\assets\test1-3.png)

输入 `3` 根据起飞日期查询航班。由于两趟航班均为 2026.4.23 起飞，故系统展示了两趟航班的起飞具体时间，并询问用户要查询哪一趟。

![test1-4](.\assets\test1-4.png)

输入 `3` 根据终点站进行查询，随后选择预订 FK9178 号航班，用户为 `neutralized`，预订了 3 张机票；

选择对该航班进行退订，用户为 `neuze`，但之前并没有该用户的预订信息，所以系统返回了一个 ERROR。

![test1-5](.\assets\test1-5.png)

输入 `1`，在控制台中手动输入新航班 MU1145 的信息，并根据终点站查询了该航班，随后用户 `neuze` 预订了 4 张机票。

![test1-6](.\assets\test1-6.png)

输入 `3`，对刚才的航班进行了查询。由于刚才的预订，此时余票数量变为了 296。尝试对该航班进行退订，再次查询余票数量：

![test1-7](.\assets\test1-7.png)

此时余票数量回到了 300。同时，由于 `neuze` 尝试取消预订 5 张机票，但他只预订过 4 张，所以系统发出了提醒，并且把 4 张机票全部取消了预订。

![test1-12](.\assets\test1-12.png)

最后，用户让系统将目前的航班信息输出到指定文件中，然后退出了程序。

#### 异常测试

输入不存在的选项：

![test1-8](.\assets\test1-8.png)

航班号格式错误：

![test1-9](.\assets\test1-9.png)

起飞/到达时间格式错误：

![test1-10](.\assets\test1-10.png)

查询失败：

![test1-11](.\assets\test1-11.png)

### 测试用例2（用 `tests` 目录下的 `generate.bat` 生成）

用于测试大规模输入/输出。

`generate.bat` 运行结果：

![test2-1](.\assets\test2-1.png)

为保证 CSV 文件被正确读入，我们选取文件靠后的一趟航班进行查询：

![test2-2](.\assets\test2-2.png)

![test2-3](.\assets\test2-3.png)

SU2206 号航班位于图中靠后的位置。此时输入 ID 进行二次查询，并尝试预订 114 张机票，系统返回了一个错误，原因是该航班只有 108 的票容量，无法进行这次预订。

![test2-4](.\assets\test2-4.png)

更多运行截图限于篇幅不在此展示。可以得出结论，程序在 $n = 10000$ 规模的随机数据上运行效果良好。

### 结论

程序在小规模和较大规模数据上均运行良好。
