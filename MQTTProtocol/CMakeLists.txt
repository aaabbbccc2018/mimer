# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mqtter)
# 添加宏REVERSED
ADD_DEFINITIONS(-DREVERSED=0)
# 指定include 路径
include_directories(.)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 指定生成目标
add_executable(mqtter ${DIR_SRCS})

# 启用测试
enable_testing()
# 测试程序是否成功运行
foreach (index RANGE 1 14)
	add_test (packet_type_${index}_dried mqtter ${index} 1)
	add_test (packet_type_${index}_official mqtter ${index} 0)
endforeach ()
