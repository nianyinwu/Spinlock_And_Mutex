# Spinlock & Mutex

評估 Spinlock與Mutex在不同參數下的效能表現  

Input :   
第一個參數為thread的數量，  
第二個、第三個和第四個參數分別是 critical section 大小、non-critical section 大小及每個 thread 總共要上鎖的數。  
（利用指令time得到程式執行時間）
