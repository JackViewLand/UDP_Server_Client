# Echo Server and Client
這是一個簡單的 UDP 回應伺服器以及用戶端程式。該伺服器會在特定的 IP 和埠上監聽 UDP 訊息，然後將相同的訊息回覆給客戶端。用戶端程式可以向伺服器發送 UDP 訊息，並顯示回應的訊息（訊息、IP 和埠是可配置的）。

## Server
* 伺服器程式碼位於 server.c。
* 在編譯並運行該程式之前，請先確保您的系統已經安裝了 GCC 編譯器。
* 在終端機中執行以下命令以編譯並運行伺服器：
編譯
```code
gcc server.c -o server
```
執行
```code
./server <IP> <port>
```
其中 <IP> 是指要監聽的 IP 位址，<port> 是指要監聽的埠號。
*當伺服器收到訊息時，它會將該訊息回應給客戶端。

## Client
* 用戶端程式碼位於 client.c。
* 在編譯並運行該程式之前，請先確保您的系統已經安裝了 GCC 編譯器。
* 在終端機中執行以下命令以編譯並運行用戶端程式：
編譯
```code
gcc client.c -o client
```
* UDP_Client.c:(.text+0x374): undefined reference to `fmin'
```
gcc UDP_Client.c -o UDP_Client -lm
```
執行
```code
./client <IP> <port> <message>
```
其中 <IP> 是指要連接的伺服器 IP 位址，<port> 是指要連接的伺服器埠號，<message> 是要傳送的訊息。
* 用戶端會將訊息傳送給伺服器，並顯示伺服器回應的訊息。
* 用戶端支援最大重試次數的設定，例如如果設定 max-retry = 10，用戶端將最多重試 10 次。每次重試之間的時間間隔應遵循指數備份演算法：
```
multiplier = 2
base = 500ms
max wait_interval = 8s
```
* 當用戶端接收到回應訊息後，它會以退出代碼 0 結束程式。如果達到了最大重試次數，則以退出代碼 1 結束程式。
