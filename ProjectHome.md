# 简介 #
RMupdate是一个用于由RPG Maker XP/VX（以下简称RM）制作并发布游戏的在线更新程序，该程序可以使游戏制作者在发布一个资源经过打包加密的游戏时免去后续发布该游戏的补丁、更新、资料片而需要重新发布体积臃肿的加密资源包的麻烦。

RMupdate包含两个基本组件——RMupdater和RMupdateManager，前者为玩家使用的更新程序，后者为开发者使用的用以制作更新补丁的管理程序。

RMupdate原生跨平台的哦～可以在Linux下使用Linux版本的更新管理程序或更新程序。更新程序将游戏更新回来以后会调用wine来运行游戏。

# 功能概述 #
该程序会自动从网上获取最新的更新列表，在确认有更新的情况下则获取相关更新文件并下载到本地，自动将更新文件添加覆盖到已存在的RM加密资源包内（[game.rgssad/game.rgss2a](http://code.google.com/p/rmupdate/wiki/rgssadrgss2a)）。

# 目前状态 #
主要功能已经完成，现在进行小范围测试。

游戏开发人员使用的管理工具——[RMupdateManager](http://code.google.com/p/rmupdate/wiki/RMupdateManager)

随游戏发布的更新工具——[RMupdater](http://code.google.com/p/rmupdate/wiki/RMupdater)

# 附加授权说明 #
该项目代码使用GPLv3进行发布，但其中涉及到加密部分的函数代码可不受GPLv3的约束。加密部分的代码可由使用者自行决定使用何种许可协议，也可以选择封闭加密部分的源代码。

加密部分的函数代码指以下四个函数的实现代码：

```
void* encrypt_file_content(void* file_content_buffer, long buffer_size, long& buffer_size_rev);
void* decrypt_file_content(void* content_buffer, long buffer_size, long& buffer_size_rev);
char* encrypt_file_path(const char* filepath);
char* decrypt_file_path(const char* filepath_chiper);
```