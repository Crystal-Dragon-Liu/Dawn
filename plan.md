# 说明

(TODO) 表示待完成任务

other 标签下的任务完成即删除

# 开发

## other

辅助设施：
- LOG宏
  - (TODO)需要能够接收任意类型和数量的参数
  - (TODO)能够打印例如时间等更多的信息，并且能够控制打印内容

(TODO)缓冲池的资源管理类

## 存储层

### 磁盘管理

总体功能：
- 对内负责初始化磁盘管理的元数据
- 对外负责普通页面的读写分配和回收
- 对外负责日志页面的读写

任务：
- 磁盘管理初始化
  - 元数据文件存在的初始化
  - 元数据文件不存在的初始化
- 普通页面管理
  - 读页面
  - 写页面
  - 分配新页面
  - 释放页面
- (TODO)并发使用DiskManager(待计划)
- (TODO)日志页面管理(待计划)
- (TODO)定期裁剪文件大小(待计划)
- (TODO)对元数据文件的logging和recovery(待计划)

### 页面布局

## 缓冲池

总体功能：
- 对外分配和回收页面，外部向BMP请求或者归还页面，后者把页面给它或者处理归还页面
- 对内决定页面的去留，视情况将页面从内存删除并冲刷回磁盘

任务：
- clock replacer的实现
- buffer pool manager的实现
   - (TODO)对日志和事务的支持(未来实现)

## 索引层

总体功能：
- 将表中的数据按照某种方式有规律的组织起来，方便查找
- 表数据在更新的时候本层也做相应的更新，所以对外开放增删查改接口

任务：
- 完成哈希法，使用链表法解决冲突机制(InnoDB也是如此)

Tips：有关事务和日志等功能应该被嵌入其中，但是考虑到方便就暂时没有实现，留作以后重写

## 执行器

待计划

## 查询计划

待计划

## 数据与系统管理

总体功能：
- 管理系统运行过程中需要的元数据
- 实现表的增删查改
- 实现数据的简单增删改查
- 数据类型描述和数据实体

任务：
- 收集系统的所有零部件
  - 数据库管理器(DBManager)
- 记录系统运行时所需的信息
  - 总目录(Catalog)
  - 表系统目录(CatalogTable)
- 表与表相关元数据的设计
  - 表元(TableMetaData)
  - (TODO)表管理(Table)
  - 表页(TablePage)
  - 表视图(TableSchema)
  - 元组(Tuple)
  - 列(Column)
  - 定位(RID)
- Type抽象类(目前只实现这四种类型，暂时没必要实现更多的类型)
  - Bool
  - Integer
  - Decimal
  - Char
  - (TODO)Varchar (这个有点复杂，暂时不做)
- Value类

#######################################################################

# 测试

所有功能都未经过并发测试
(TODO)并发测试

## other

打开内存泄漏检测

磁盘管理测试的辅助：
- 创建几个已经有内容的元数据文件和相关的.db .log文件，方便以后反复使用

## 存储层测试

### 磁盘管理测试

- 磁盘管理初始化
  - 元数据文件存在的初始化
    - 正确收集page id信息
  - 元数据文件不存在的初始化
- 普通页面管理
  - 读、写、分配和释放页面
  - (TODO)并发分配和释放页面 

## 缓冲池测试

因为实现还很不稳定所以接口要封装

- clock replacer非并发
  - unpin一些页框，能够把它们全部逐出
  - unpin一些页框，pin其中的一部分，剩下的全部逐出
  - 逐出时没有可逐出页框，等待，放入页框后成功逐出
- buffer pool manager非并发
  - 针对一个页面作如下操作：创建，写，驱逐，拿，读，写，冲刷，直读磁盘，最后删除 （TODO) flush_all
  - 从缓冲池获取若干页面，pin住其中的一部分并写入然后再从缓冲池大量获取并归还，保证pinned页面仍在缓冲池中，unpinned不在缓冲池，并且内容已经被写回了磁盘

- (TODO)clock replacer并发测试
- (TODO)buffer pool manager并发测试

## 索引层测试

因为实现还很不稳定所以接口要封装

## 执行器测试

待计划

## 查询计划测试

待计划

## 数据与系统管理测试

- 系统目录(Catalog)测试
  - 实现系统目录的基本创建和保存

- 表系统目录(CatalogTable)测试
  - 实现表的创建和删除，并且保证操作的持久化

- 表(Table)测试
  - (TODO)对表中元组的增删改查

- 表页(TablePage)测试
  - 对表页中元组增删改查

- 数据与数据类型(Tuple, Value, Type)
  - Tuple的基本功能
  - (TODO Decimal和Char)基本运算
  - (TODO)序列化和反序列化

- (TODO)并发测试