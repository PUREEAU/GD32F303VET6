/*
 * FreeRTOS 内核 V11.1.0
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/******************************************************************************/
/* 硬件描述相关定义 ***********************************************************/
/******************************************************************************/

/* 大多数情况下，configCPU_CLOCK_HZ 必须设置为用于生成内核周期性 tick 中断的外设时钟频率。
 * 默认值设为 20MHz，与 QEMU 演示设置匹配。您的应用程序肯定需要不同的值，请正确设置。
 * 这通常（但不总是）等于主系统时钟频率。 */
#if defined(__ICCARM__)||defined(__CC_ARM)||defined(__GNUC__)
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif
#define configCPU_CLOCK_HZ    (SystemCoreClock)
/* configSYSTICK_CLOCK_HZ 是 ARM Cortex-M 端口可选参数。
 *
 * 默认情况下，ARM Cortex-M 端口使用 Cortex-M SysTick 定时器生成 RTOS tick 中断。
 * 大多数 Cortex-M MCU 以与 MCU 本身相同的频率运行 SysTick 定时器 —— 这种情况下不需要定义
 * configSYSTICK_CLOCK_HZ，应保持注释状态。如果 SysTick 定时器时钟频率与 MCU 内核不同，
 * 则照常将 configCPU_CLOCK_HZ 设为 MCU 时钟频率，并将 configSYSTICK_CLOCK_HZ 设为 SysTick 时钟频率。
 * 如果未定义，则不使用。默认未定义（注释掉）。如果需要此值，请取消注释并设置为合适的值。 */

/*
 #define configSYSTICK_CLOCK_HZ                  [平台特定]
 */

/******************************************************************************/
/* 调度行为相关定义 ***********************************************************/
/******************************************************************************/

/* configTICK_RATE_HZ 设置 tick 中断的频率（Hz），通常根据 configCPU_CLOCK_HZ 计算。 */
#define configTICK_RATE_HZ                         ( ( TickType_t ) 1000 )

/* 设置 configUSE_PREEMPTION 为 1 使用抢占式调度，为 0 使用协作式调度。
 * 参见 https://www.freertos.org/single-core-amp-smp-rtos-scheduling.html */
#define configUSE_PREEMPTION                       1

/* 设置 configUSE_TIME_SLICING 为 1，使调度器在每个 tick 中断时在相同优先级的就绪态任务之间切换。
 * 设为 0 可防止调度器仅因为 tick 中断就在就绪态任务之间切换。
 * 参见 https://freertos.org/single-core-amp-smp-rtos-scheduling.html */
#define configUSE_TIME_SLICING                     0

/* 设置 configUSE_PORT_OPTIMISED_TASK_SELECTION 为 1，使用针对目标硬件指令集优化的算法选择下一个运行的任务
 * （通常使用前导零计数汇编指令）。设为 0 则使用适用于所有 FreeRTOS 端口的通用 C 算法。
 * 并非所有 FreeRTOS 端口都有此选项。如果未定义，默认为 0。 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0

/* 设置 configUSE_TICKLESS_IDLE 为 1 使用低功耗 tickless 模式，设为 0 则始终保持 tick 中断运行。
 * 并非所有 FreeRTOS 端口都支持 tickless 模式。参见 https://www.freertos.org/low-power-tickless-rtos.html
 * 如果未定义，默认为 0。 */
#define configUSE_TICKLESS_IDLE                    0

/* configMAX_PRIORITIES 设置可用任务优先级的数量。任务可被分配 0 到 (configMAX_PRIORITIES-1) 的优先级。
 * 0 是最低优先级。 */
#define configMAX_PRIORITIES                       5

/* configMINIMAL_STACK_SIZE 定义空闲任务使用的栈大小（以字为单位，而非字节！）。
 * 内核不会将此常量用于任何其他目的。示例应用程序使用此常量使示例在多种硬件架构间具有一定的可移植性。 */
#define configMINIMAL_STACK_SIZE                   128

/* configMAX_TASK_NAME_LEN 设置任务可读名称的最大长度（字符数），包括 NULL 终止符。 */
#define configMAX_TASK_NAME_LEN                    16

/* 时间以“tick”为单位测量 —— 即从 RTOS 内核启动以来 tick 中断执行的次数。
 * tick 计数保存在类型为 TickType_t 的变量中。
 *
 * configTICK_TYPE_WIDTH_IN_BITS 控制 TickType_t 的类型（因此也控制位宽）：
 *
 * 定义为 TICK_TYPE_WIDTH_16_BITS 会使 TickType_t 被定义为无符号 16 位类型。
 * 定义为 TICK_TYPE_WIDTH_32_BITS 会使 TickType_t 被定义为无符号 32 位类型。
 * 定义为 TICK_TYPE_WIDTH_64_BITS 会使 TickType_t 被定义为无符号 64 位类型。 */
#define configTICK_TYPE_WIDTH_IN_BITS              TICK_TYPE_WIDTH_32_BITS

/* 设置 configIDLE_SHOULD_YIELD 为 1，如果存在可以运行的空闲优先级（优先级 0）应用程序任务，
 * 则让空闲任务让步给该应用程序任务。设为 0 则让空闲任务使用其完整的时间片。
 * 如果未定义，默认为 1。 */
#define configIDLE_SHOULD_YIELD                    1

/* 每个任务都有一个任务通知数组。
 * configTASK_NOTIFICATION_ARRAY_ENTRIES 设置数组中的索引数量。
 * 参见 https://www.freertos.org/RTOS-task-notifications.html 如果未定义，默认为 1。 */
#define configTASK_NOTIFICATION_ARRAY_ENTRIES      1

/* configQUEUE_REGISTRY_SIZE 设置可从队列注册表引用的队列和信号量的最大数量。
 * 仅在使用内核感知调试器时需要。如果未定义，默认为 0。 */
#define configQUEUE_REGISTRY_SIZE                  0

/* 设置 configENABLE_BACKWARD_COMPATIBILITY 为 1，将旧版 FreeRTOS 中的函数名和数据类型映射到最新等效项。
 * 如果未定义，默认为 1。 */
#define configENABLE_BACKWARD_COMPATIBILITY        0

/* 每个任务都有自己的指针数组，可用作线程本地存储。
 * configNUM_THREAD_LOCAL_STORAGE_POINTERS 设置数组中的索引数量。
 * 参见 https://www.freertos.org/thread-local-storage-pointers.html 如果未定义，默认为 0。 */
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS    0

/* 当 configUSE_MINI_LIST_ITEM 设为 0 时，MiniListItem_t 和 ListItem_t 相同。
 * 设为 1 时，MiniListItem_t 比 ListItem_t 少 3 个字段，可节省一些 RAM，但会违反某些编译器依赖的严格别名规则以进行优化。
 * 如果未定义，默认为 1。 */
#define configUSE_MINI_LIST_ITEM                   1

/* 设置 xTaskCreate() 参数中用于指定任务栈大小的类型。同样的类型也用于其他各种 API 中返回栈使用信息。
 * 如果未定义，默认为 size_t。 */
#define configSTACK_DEPTH_TYPE                     size_t

/* configMESSAGE_BUFFER_LENGTH_TYPE 设置存储写入 FreeRTOS 消息缓冲区的每条消息长度的类型（长度也会写入消息缓冲区）。
 * 如果未定义，默认为 size_t —— 但如果消息长度永远不会超过 uint8_t 能容纳的范围，这可能会浪费空间。 */
#define configMESSAGE_BUFFER_LENGTH_TYPE           size_t

/* 如果 configHEAP_CLEAR_MEMORY_ON_FREE 设为 1，则使用 pvPortMalloc() 分配的内存块在通过 vPortFree() 释放时将被清零。
 * 如果未定义，默认为 0。 */
#define configHEAP_CLEAR_MEMORY_ON_FREE            1

/* vTaskList 和 vTaskGetRunTimeStats API 接收一个缓冲区作为参数，并假定该缓冲区的长度为 configSTATS_BUFFER_MAX_LENGTH。
 * 如果未定义，默认为 0xFFFF。
 * 新应用程序建议改用 vTaskListTasks 和 vTaskGetRunTimeStatistics API，并显式提供缓冲区长度以避免内存损坏。 */
#define configSTATS_BUFFER_MAX_LENGTH              0xFFFF

/* 设置 configUSE_NEWLIB_REENTRANT 为 1，为每个任务分配一个 newlib reent 结构体。
 * 设为 0 则不支持 newlib reent 结构体。如果未定义，默认为 0。
 *
 * 注意：Newlib 的支持是应大众需求添加的，但 FreeRTOS 维护者自身并未使用或测试。FreeRTOS 不对由此产生的 newlib 行为负责。
 * 用户必须熟悉 newlib，并提供必要的系统级存根实现。请注意（在撰写本文时），当前的 newlib 设计实现了一个系统级的 malloc()，
 * 必须为其提供锁。 */
#define configUSE_NEWLIB_REENTRANT                 0

/******************************************************************************/
/* 软件定时器相关定义 *********************************************************/
/******************************************************************************/

/* 设置 configUSE_TIMERS 为 1 以在构建中包含软件定时器功能，设为 0 则排除。
 * 如果 configUSE_TIMERS 设为 1，则必须将 FreeRTOS/source/timers.c 源文件包含在构建中。
 * 如果未定义，默认为 0。参见 https://www.freertos.org/RTOS-software-timer.html */
#define configUSE_TIMERS                1

/* configTIMER_TASK_PRIORITY 设置定时器任务使用的优先级。仅在 configUSE_TIMERS 设为 1 时使用。
 * 定时器任务是标准的 FreeRTOS 任务，因此其优先级与其他任务一样设置。
 * 参见 https://www.freertos.org/RTOS-software-timer-service-daemon-task.html 仅在 configUSE_TIMERS 设为 1 时使用。 */
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1 )

/* configTIMER_TASK_STACK_DEPTH 设置分配给定时器任务的栈大小（以字为单位，而非字节！）。
 * 定时器任务是标准的 FreeRTOS 任务。
 * 参见 https://www.freertos.org/RTOS-software-timer-service-daemon-task.html 仅在 configUSE_TIMERS 设为 1 时使用。 */
#define configTIMER_TASK_STACK_DEPTH    configMINIMAL_STACK_SIZE

/* configTIMER_QUEUE_LENGTH 设置用于向定时器任务发送命令的队列长度（队列可以容纳的离散项数）。
 * 参见 https://www.freertos.org/RTOS-software-timer-service-daemon-task.html 仅在 configUSE_TIMERS 设为 1 时使用。 */
#define configTIMER_QUEUE_LENGTH        10

/******************************************************************************/
/* 事件组相关定义 *************************************************************/
/******************************************************************************/

/* 设置 configUSE_EVENT_GROUPS 为 1 以在构建中包含事件组功能，设为 0 则排除。
 * 如果 configUSE_EVENT_GROUPS 设为 1，则必须将 FreeRTOS/source/event_groups.c 源文件包含在构建中。
 * 如果未定义，默认为 1。 */

#define configUSE_EVENT_GROUPS    1

/******************************************************************************/
/* 流缓冲区相关定义 ***********************************************************/
/******************************************************************************/

/* 设置 configUSE_STREAM_BUFFERS 为 1 以在构建中包含流缓冲区功能，设为 0 则排除。
 * 如果 configUSE_STREAM_BUFFERS 设为 1，则必须将 FreeRTOS/source/stream_buffer.c 源文件包含在构建中。
 * 如果未定义，默认为 1。 */

#define configUSE_STREAM_BUFFERS    1

/******************************************************************************/
/* 内存分配相关定义 ***********************************************************/
/******************************************************************************/

/* 设置 configSUPPORT_STATIC_ALLOCATION 为 1 以在构建中包含使用静态分配内存创建 FreeRTOS 对象（任务、队列等）的 API 函数。
 * 设为 0 则排除创建静态分配对象的能力。如果未定义，默认为 0。
 * 参见 https://www.freertos.org/Static_Vs_Dynamic_Memory_Allocation.html */
#define configSUPPORT_STATIC_ALLOCATION              1

/* 设置 configSUPPORT_DYNAMIC_ALLOCATION 为 1 以在构建中包含使用动态分配内存创建 FreeRTOS 对象（任务、队列等）的 API 函数。
 * 设为 0 则排除创建动态分配对象的能力。如果未定义，默认为 1。
 * 参见 https://www.freertos.org/Static_Vs_Dynamic_Memory_Allocation.html */
#define configSUPPORT_DYNAMIC_ALLOCATION             1

/* 当构建中包含 heap_1.c、heap_2.c 或 heap_4.c 时，configTOTAL_HEAP_SIZE 设置 FreeRTOS 堆的总大小（以字节为单位）。
 * 该值默认为 4096 字节，但必须针对每个应用程序进行调整。注意堆将出现在 .bss 段中。
 * 参见 https://www.freertos.org/a00111.html */
#define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 20 * 1024 ) )

/* 设置 configAPPLICATION_ALLOCATED_HEAP 为 1，让应用程序分配用作 FreeRTOS 堆的数组。
 * 设为 0 则让链接器分配用作 FreeRTOS 堆的数组。如果未定义，默认为 0。 */
#define configAPPLICATION_ALLOCATED_HEAP             0

/* 设置 configSTACK_ALLOCATION_FROM_SEPARATE_HEAP 为 1，从 FreeRTOS 堆以外的地方分配任务栈。
 * 如果您希望确保栈位于快速内存中，这很有用。设为 0 则让任务栈来自标准的 FreeRTOS 堆。
 * 如果设为 1，应用程序编写者必须提供 pvPortMallocStack() 和 vPortFreeStack() 的实现。
 * 如果未定义，默认为 0。 */
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP    0

/* 设置 configENABLE_HEAP_PROTECTOR 为 1，在 heap_4.c 和 heap_5.c 中启用堆内部块指针的边界检查和混淆，以帮助捕获指针损坏。
 * 如果未定义，默认为 0。 */
#define configENABLE_HEAP_PROTECTOR                  0

/******************************************************************************/
/* 中断嵌套行为配置 ***********************************************************/
/******************************************************************************/

/* configKERNEL_INTERRUPT_PRIORITY 设置 tick 和上下文切换中断的优先级。
 * 并非所有 FreeRTOS 端口都支持。有关 ARM Cortex-M 设备的具体信息，请参见
 * https://www.freertos.org/RTOS-Cortex-M3-M4.html */
#define configKERNEL_INTERRUPT_PRIORITY          15

/* configMAX_SYSCALL_INTERRUPT_PRIORITY 设置中断优先级的上限，高于此优先级的中断不能调用 FreeRTOS API 函数。
 * 高于此优先级的中断永远不会被禁用，因此永远不会被 RTOS 活动延迟。默认值设为最高中断优先级 (0)。
 * 并非所有 FreeRTOS 端口都支持。有关 ARM Cortex-M 设备的具体信息，请参见
 * https://www.freertos.org/RTOS-Cortex-M3-M4.html */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY     5

/* configMAX_API_CALL_INTERRUPT_PRIORITY 是 configMAX_SYSCALL_INTERRUPT_PRIORITY 的另一个名称 —— 使用的名称取决于 FreeRTOS 端口。 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY    5

/******************************************************************************/
/* 钩子和回调函数相关定义 *****************************************************/
/******************************************************************************/

/* 将以下 configUSE_* 常量设置为 1 以在构建中包含指定的钩子功能，设为 0 则排除。
 * 对于任何设为 1 的功能，应用程序编写者负责提供钩子函数。
 * 参见 https://www.freertos.org/a00016.html */
#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MALLOC_FAILED_HOOK          0
#define configUSE_DAEMON_TASK_STARTUP_HOOK    0

/* 设置 configUSE_SB_COMPLETED_CALLBACK 为 1，为每个流缓冲区或消息缓冲区实例启用发送和接收完成回调。
 * 当该选项设为 1 时，可以使用 API xStreamBufferCreateWithCallback() 和 xStreamBufferCreateStaticWithCallback()
 * （以及消息缓冲区的类似 API）创建带有应用程序提供回调的流缓冲区或消息缓冲区实例。
 * 如果未定义，默认为 0。 */
#define configUSE_SB_COMPLETED_CALLBACK       0

/* 设置 configCHECK_FOR_STACK_OVERFLOW 为 1 或 2，让 FreeRTOS 在上下文切换时检查栈溢出。
 * 设为 0 则不检查栈溢出。
 * 如果 configCHECK_FOR_STACK_OVERFLOW 为 1，则仅检查在任务上下文保存到其栈时栈指针是否越界 —— 这很快但效果较差。
 * 如果为 2，则检查写入任务栈末尾的模式是否被覆盖。这较慢，但能捕获大多数（但不是全部）栈溢出。
 * 当 configCHECK_FOR_STACK_OVERFLOW 设为 1 时，应用程序编写者必须提供栈溢出回调。
 * 参见 https://www.freertos.org/Stacks-and-stack-overflow-checking.html 如果未定义，默认为 0。 */
#define configCHECK_FOR_STACK_OVERFLOW        0

/******************************************************************************/
/* 运行时和任务统计信息收集相关定义 *******************************************/
/******************************************************************************/

/* 设置 configGENERATE_RUN_TIME_STATS 为 1，让 FreeRTOS 收集每个任务使用的处理时间数据。
 * 设为 0 则不收集数据。如果设为 1，应用程序编写者需要提供时钟源。
 * 如果未定义，默认为 0。参见 https://www.freertos.org/rtos-run-time-stats.html */
#define configGENERATE_RUN_TIME_STATS           0

/* 设置 configUSE_TRACE_FACILITY 为 1，在任务结构体中包含额外的成员，供追踪和可视化功能及工具使用。
 * 设为 0 则从结构体中排除这些额外信息。如果未定义，默认为 0。 */
#define configUSE_TRACE_FACILITY                0

/* 设为 1 以在构建中包含 vTaskList() 和 vTaskGetRunTimeStats() 函数，设为 0 则排除。
 * 这两个函数引入了字符串格式化函数的依赖，否则这些函数不会存在 —— 因此它们被分开。
 * 如果未定义，默认为 0。 */
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/******************************************************************************/
/* 协程相关定义 ***************************************************************/
/******************************************************************************/

/* 设置 configUSE_CO_ROUTINES 为 1 以在构建中包含协程功能，设为 0 则排除。
 * 要包含协程，必须将 croutine.c 包含在项目中。如果未定义，默认为 0。 */
#define configUSE_CO_ROUTINES              0

/* configMAX_CO_ROUTINE_PRIORITIES 定义应用程序协程可用的优先级数量。
 * 任意数量的协程可以共享同一优先级。如果未定义，默认为 0。 */
#define configMAX_CO_ROUTINE_PRIORITIES    1

/******************************************************************************/
/* 调试辅助 *********************************************************/
/******************************************************************************/

/* configASSERT() 与标准 C assert() 具有相同的语义。
 * 它可以定义为在断言失败时采取行动，或者完全未定义（即注释或删除定义）以完全移除断言。
 * configASSERT() 可以定义为任何你想要的，例如在断言失败时调用一个传递失败断言文件名和行号的函数
 * （例如 "vAssertCalled( __FILE__, __LINE__ )"），或者简单地禁用中断并坐在一个循环中以停止所有执行，便于在调试器中查看失败行。 */
//#define configASSERT( x )         \
//    if( ( x ) == 0 )              \
//    {                             \
//        taskDISABLE_INTERRUPTS(); \
//        for( ; ; )                \
//        ;                         \
//    }

/******************************************************************************/
/* FreeRTOS MPU 特定定义 ******************************************************/
/******************************************************************************/

/* 如果 configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 设为 1，
 * 应用程序编写者可以提供在特权模式下执行的函数。
 * 参见：https://www.freertos.org/a00110.html#configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
 * 如果未定义，默认为 0。仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。 */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0

/* configTOTAL_MPU_REGIONS 设置目标硬件上实现的 MPU 区域数量。通常为 8 或 16。
 * 仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。如果未定义，默认为 8。 */
#define configTOTAL_MPU_REGIONS                                   8

/* configTEX_S_C_B_FLASH 允许应用程序编写者覆盖覆盖 Flash 的 MPU 区域的 TEX、可共享（S）、可缓存（C）和可缓冲（B）位的默认值。
 * 如果未定义，默认为 0x07UL（即 TEX=000，S=1，C=1，B=1）。仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。 */
#define configTEX_S_C_B_FLASH                                     0x07UL

/* configTEX_S_C_B_SRAM 允许应用程序编写者覆盖覆盖 RAM 的 MPU 区域的 TEX、可共享（S）、可缓存（C）和可缓冲（B）位的默认值。
 * 如果未定义，默认为 0x07UL（即 TEX=000，S=1，C=1，B=1）。仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。 */
#define configTEX_S_C_B_SRAM                                      0x07UL

/* 设置 configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY 为 0，以防止任何源自内核代码之外的权限提升。
 * 设为 1 则允许应用程序任务提升权限。如果未定义，默认为 1。
 * 仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。 */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY               1

/* 设置 configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS 为 1，允许非特权任务进入临界区（即屏蔽中断）。
 * 设为 0 则阻止非特权任务进入临界区。如果未定义，默认为 1。
 * 仅由 FreeRTOS Cortex-M MPU 端口使用，而非标准 ARMv7-M Cortex-M 端口。 */
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS                0

/* FreeRTOS 内核版本 10.6.0 引入了新的 v2 MPU 包装器，即 mpu_wrappers_v2.c。
 * 设置 configUSE_MPU_WRAPPERS_V1 为 0 以使用新的 v2 MPU 包装器，设为 1 以使用旧的 v1 MPU 包装器（mpu_wrappers.c）。
 * 如果未定义，默认为 0。 */
#define configUSE_MPU_WRAPPERS_V1                                 0

/* 使用 v2 MPU 包装器时，设置 configPROTECTED_KERNEL_OBJECT_POOL_SIZE 为应用程序中内核对象的总数，
 * 包括任务、队列、信号量、互斥量、事件组、定时器、流缓冲区和消息缓冲区。
 * 应用程序在任何时候都不能拥有超过 configPROTECTED_KERNEL_OBJECT_POOL_SIZE 个内核对象。 */
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                   10

/* 使用 v2 MPU 包装器时，设置 configSYSTEM_CALL_STACK_SIZE 为系统调用栈的大小（以字为单位）。
 * 每个任务都有一个静态分配的内存缓冲区，其大小为此值，用作执行系统调用的栈。
 * 例如，如果 configSYSTEM_CALL_STACK_SIZE 定义为 128 且应用程序中有 10 个任务，
 * 则用于系统调用栈的总内存量为 128 * 10 = 1280 字。 */
#define configSYSTEM_CALL_STACK_SIZE                              128

/* 使用 v2 MPU 包装器时，设置 configENABLE_ACCESS_CONTROL_LIST 为 1 以启用访问控制列表（ACL）功能。
 * 当 ACL 启用时，非特权任务默认无法访问除自身以外的任何内核对象。
 * 应用程序编写者需要显式使用提供的 API 授予非特权任务对所需内核对象的访问权限。
 * 如果未定义，默认为 0。 */
#define configENABLE_ACCESS_CONTROL_LIST                          1

/******************************************************************************/
/* SMP（对称多处理）特定配置定义 **********************************************/
/******************************************************************************/

/* 设置 configNUMBER_OF_CORES 为可用的处理器核心数量。如果未定义，默认为 1。 */

/*
 #define configNUMBER_OF_CORES                     [可用核心数]
 */

/* 当使用 SMP（即 configNUMBER_OF_CORES > 1）时，设置 configRUN_MULTIPLE_PRIORITIES 为 0，
 * 仅当任务优先级不相等时才允许多个任务同时运行，从而保持低优先级任务在高优先级任务可运行时永远不会运行的原则。
 * 如果 configRUN_MULTIPLE_PRIORITIES 设为 1，则不同优先级的多个任务可以同时运行 —— 因此高优先级和低优先级任务可能在不同的核心上同时运行。 */
#define configRUN_MULTIPLE_PRIORITIES             0

/* 当使用 SMP（即 configNUMBER_OF_CORES > 1）时，设置 configUSE_CORE_AFFINITY 为 1 以启用核心亲和性功能。
 * 当核心亲和性功能启用时，可以使用 vTaskCoreAffinitySet 和 vTaskCoreAffinityGet API 来设置和检索任务可以在哪些核心上运行。
 * 如果 configUSE_CORE_AFFINITY 设为 0，则 FreeRTOS 调度器可以自由地在任何可用核心上运行任何任务。 */
#define configUSE_CORE_AFFINITY                   0

/* 当使用启用了核心亲和性功能的 SMP 时，设置 configTASK_DEFAULT_CORE_AFFINITY 以更改在创建时未指定亲和性掩码的任务的默认核心亲和性掩码。
 * 将该宏定义为 1 会使此类任务在核心 0 上运行，定义为 (1 << portGET_CORE_ID()) 会使它们在当前核心上运行。
 * 如果不支持在核心之间交换任务（例如 Tricore），或者需要控制遗留代码，此配置值很有用。
 * 如果未定义，默认为 tskNO_AFFINITY。 */
#define configTASK_DEFAULT_CORE_AFFINITY          tskNO_AFFINITY

/* 当使用 SMP（即 configNUMBER_OF_CORES > 1）时，如果 configUSE_TASK_PREEMPTION_DISABLE 设为 1，
 * 则可以使用 vTaskPreemptionDisable 和 vTaskPreemptionEnable API 将单个任务设置为抢占模式或协作模式。 */
#define configUSE_TASK_PREEMPTION_DISABLE         0

/* 当使用 SMP（即 configNUMBER_OF_CORES > 1）时，设置 configUSE_PASSIVE_IDLE_HOOK 为 1，
 * 允许应用程序编写者使用被动空闲任务钩子添加后台功能，而无需单独任务的额外开销。
 * 如果未定义，默认为 0。 */
#define configUSE_PASSIVE_IDLE_HOOK               0

/* 当使用 SMP（即 configNUMBER_OF_CORES > 1）时，configTIMER_SERVICE_TASK_CORE_AFFINITY
 * 允许应用程序编写者设置 RTOS 守护进程/定时器服务任务的核心亲和性。
 * 如果未定义，默认为 tskNO_AFFINITY。 */
#define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY


/******************************************************************************/
/* ARMv8-M 安全侧端口相关定义 *************************************************/
/******************************************************************************/

/* secureconfigMAX_SECURE_CONTEXTS 定义可以调用 ARMv8-M 芯片安全侧的最大任务数量。任何其他端口不使用。 */
#define secureconfigMAX_SECURE_CONTEXTS        5

/* 定义内核提供的 vApplicationGetIdleTaskMemory() 和 vApplicationGetTimerTaskMemory() 实现，
 * 以提供空闲任务和定时器任务使用的内存。应用程序可以通过将 configKERNEL_PROVIDED_STATIC_MEMORY 设为 0 或将其保持未定义，
 * 来提供自己的 vApplicationGetIdleTaskMemory() 和 vApplicationGetTimerTaskMemory() 实现。 */
#define configKERNEL_PROVIDED_STATIC_MEMORY    1

/******************************************************************************/
/* ARMv8-M 端口特定配置定义 ***************************************************/
/******************************************************************************/

/* 在非安全侧运行 FreeRTOS 时，设置 configENABLE_TRUSTZONE 为 1 以启用 FreeRTOS ARMv8-M 端口中的 TrustZone 支持，
 * 这允许非安全的 FreeRTOS 任务调用从安全侧导出的（非安全可调用）函数。 */
#define configENABLE_TRUSTZONE            1

/* 如果应用程序编写者不想使用 TrustZone，但硬件不支持禁用 TrustZone，则整个应用程序（包括 FreeRTOS 调度器）
 * 可以在安全侧运行，而永远不会跳转到非安全侧。为此，除了将 configENABLE_TRUSTZONE 设为 0 外，
 * 还要将 configRUN_FREERTOS_SECURE_ONLY 设为 1。 */
#define configRUN_FREERTOS_SECURE_ONLY    1

/* 设置 configENABLE_MPU 为 1 以启用内存保护单元（MPU），设为 0 则禁用 MPU。 */
#define configENABLE_MPU                  0

/* 设置 configENABLE_FPU 为 1 以启用浮点单元（FPU），设为 0 则禁用 FPU。 */
#define configENABLE_FPU                  0

/* 设置 configENABLE_MVE 为 1 以启用 M 配置文件向量扩展（MVE）支持，设为 0 则禁用 MVE 支持。
 * 此选项仅适用于 Cortex-M55 和 Cortex-M85 端口，因为 MVE 仅在这些架构上可用。
 * 对于 Cortex-M23、Cortex-M33 和 Cortex-M35P 端口，configENABLE_MVE 必须保持未定义或定义为 0。 */
#define configENABLE_MVE                  1

/******************************************************************************/
/* ARMv7-M 和 ARMv8-M 端口特定配置定义 ****************************************/
/******************************************************************************/

/* 设置 configCHECK_HANDLER_INSTALLATION 为 1 以启用额外的断言，验证应用程序是否正确安装了 FreeRTOS 中断处理程序。
 *
 * 应用程序可以通过以下方式之一安装 FreeRTOS 中断处理程序：
 *   1. 直接路由 - 为 SVC 调用和 PendSV 中断分别安装函数 vPortSVCHandler 和 xPortPendSVHandler。
 *   2. 间接路由 - 为 SVC 调用和 PendSV 中断安装单独的处理程序，并从这些处理程序将程序控制路由到 vPortSVCHandler 和 xPortPendSVHandler 函数。
 * 使用间接路由的应用程序必须将 configCHECK_HANDLER_INSTALLATION 设为 0。
 *
 * 如果未定义，默认为 1。 */
#define configCHECK_HANDLER_INSTALLATION    0


/******************************************************************************/
/* 包含或排除功能的定义 *******************************************************/
/******************************************************************************/

/* 将以下 configUSE_* 常量设为 1 以在构建中包含指定的功能，设为 0 则排除。 */
#define configUSE_TASK_NOTIFICATIONS           1
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES          1
#define configUSE_QUEUE_SETS                   0
#define configUSE_APPLICATION_TASK_TAG         0

/* 将以下 INCLUDE_* 常量设为 1 以包含指定的 API 函数，设为 0 则排除。
 * 大多数链接器会在常量设为 1 时仍移除未使用的函数。 */
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_xResumeFromISR                 1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_xTaskGetCurrentTaskHandle      1
#define INCLUDE_uxTaskGetStackHighWaterMark    0
#define INCLUDE_xTaskGetIdleTaskHandle         0
#define INCLUDE_eTaskGetState                  0
#define INCLUDE_xEventGroupSetBitFromISR       1
#define INCLUDE_xTimerPendFunctionCall         0
#define INCLUDE_xTaskAbortDelay                0
#define INCLUDE_xTaskGetHandle                 0
#define INCLUDE_xTaskResumeFromISR             1

/* 将 FreeRTOS 的 SVC 和 PendSV 处理函数映射到启动文件中使用的名称 */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler


#endif /* FREERTOS_CONFIG_H */