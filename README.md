# Hide-and-seek Game

## Giới thiệu

**Hide-and-Seek** là một trò chơi trò chơi được lập trình bằng **C++**, kết hợp các **thuật toán trí tuệ nhân tạo (AI)** để điều khiển hành vi của các tác nhân (*agents*) trong môi trường 2D.

Người chơi (AI) có thể đảm nhận vai trò **Seeker (người đi tìm)** và **Hider (người trốn)** trong một bản đồ có tường, vật cản, và khu vực quan sát hạn chế. Mục tiêu của Seeker là **bắt được tất cả Hider trong thời gian giới hạn**.

## 🧩 Mô tả chi tiết

### 1. Luật chơi và cơ chế hoạt động

- Bản đồ là một **hình chữ nhật hoặc hình vuông** chia thành các **ô (cell)**.  
- Các **tường (wall)** bao quanh bản đồ.  
- **Obstacles (vật cản)** có thể di chuyển được (ở các level cao).  
- **Seeker** và **Hider** mỗi bên chiếm 1 ô duy nhất.  
- Mỗi agent **có thể di chuyển 8 hướng** (lên, xuống, trái, phải và chéo).  
- Mỗi bước di chuyển **mất 1 đơn vị thời gian**.  
- **Quan sát (visibility range):** mỗi agent chỉ có thể nhìn trong phạm vi bán kính 3 ô (bị cản bởi tường và vật cản).  
- **Khi Hider bị bắt**, Seeker cần **chạm vào vị trí của Hider**.  
- **Game kết thúc** khi:
  - Seeker bắt được tất cả Hider → **Thắng**
  - Hết thời gian → **Thua**
### 2. Mức độ (Levels)

| Level | Mô tả | Ghi chú |
|:------|:------|:--------|
| **1** | 1 Hider cố định, không giới hạn thời gian. | Dễ nhất. |
| **2** | Nhiều Hider cố định, Seeker biết số lượng Hider. | Không giới hạn thời gian. |
| **3** | Hider có thể di chuyển, tầm nhìn 2 ô, các tác nhân luân phiên di chuyển. | Không giới hạn thời gian. |
| **4** | Hider có thể di chuyển vật cản trước khi game bắt đầu, Seeker có thể di chuyển vật cản trong lúc chơi. | Có giới hạn thời gian. |

---

### 3. Cách tính điểm

- Mỗi Hider bị bắt: **+20 điểm**

## 💿 Đầu vào & Đầu ra

### 🔹 Input: `map.txt`
- **Dòng 1:** `N M` – kích thước bản đồ (số hàng × số cột).  
- **N dòng tiếp theo:** ma trận bản đồ (0–3):  
  | Giá trị | Ý nghĩa |
  |:-------:|:--------|
  | 0 | Ô trống |
  | 1 | Tường |
  | 2 | Hider |
  | 3 | Seeker |
- **Các dòng cuối:** danh sách vật cản dưới dạng 4 cặp tọạ độ *(top, left, bottom, right)*.

### 🔹 Output:
- File văn bản hoặc giao diện đồ họa thể hiện:
  - Lộ trình di chuyển của Seeker.  
  - Tổng điểm và chiều dài đường đi.  
  - Các bước di chuyển được mô phỏng hoặc hiển thị theo thời gian thực.


## Cấu trúc thư mục

## ⚙️ Cấu trúc thư mục

```
gamepong/
│
├── AstarSearch.h
├── bfs_algorithm.h
├── minimax_algorithm.h          # Các thuật toán AI
│
├── figure.h
├── hider.h
├── seeker.h
├── sightrange.h                 # Định nghĩa tác nhân và logic game
│
├── Source.cpp                   # Điểm vào chương trình
│
├── Assets/                      # Tài nguyên hình ảnh, âm thanh
├── Level/
│   ├── level.h
│   ├── Level1_2/
│   ├── Level3/
│   └── Level4/                  # Dữ liệu bản đồ cho từng level
│
├── Menu/
│   ├── MenuEntry.h
│   ├── MenuLevel*.h
│   └── WinLose.h                # Giao diện menu
│
├── include/
├── lib/
├── x64/
└── Debug/
```

## 🧮 Thuật toán AI sử dụng

| Thuật toán | Vai trò |
|:------------|:--------|
| **A\*** | Tìm đường đi tối ưu giữa hai điểm (Seeker → Hider). |
| **BFS** | Dò tìm ngắn nhất trong bản đồ không trọng số. |
| **Minimax** | Quyết định chiến lược cho các tác nhân thông minh (Seeker – Hider). |


## 🖥️ Hướng dẫn build & chạy

1. Mở `gamepong.sln` trong **Visual Studio**.  
2. Chọn cấu hình `Debug` hoặc `Release`.  
3. Build và chạy dự án (`Ctrl + F5`).  
4. Chọn **Level** trong menu để bắt đầu trò chơi.  

## 🧱 Môi trường và công cụ

- **Ngôn ngữ:** C++  
- **IDE:** Visual Studio  
- **Thư viện:** SFML (Simple and Fast Multimedia Library)  
- **Hệ điều hành:** Windows 10/11  

---

## 📊 Demo

| Demo 1 | Demo 2 |
|:-------:|:-------:|
| ![demo1](demo1.png) | ![demo2](demo2.png) |

