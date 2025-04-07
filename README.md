# CppND-System-Monitor

Starter code for System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

Follow along with the classroom lesson to complete the project!

![System Monitor](images/monitor.png)

## Udacity Linux Workspace
[Udacity](https://www.udacity.com/) provides a browser-based Linux [Workspace](https://engineering.udacity.com/creating-a-gpu-enhanced-virtual-desktop-for-udacity-497bdd91a505) for students. 

You are welcome to develop this project on your local machine, and you are not required to use the Udacity Workspace. However, the Workspace provides a convenient and consistent Linux development environment we encourage you to try.

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Within the Udacity Workspace, `.student_bashrc` automatically installs ncurses every time you launch the Workspace.

If you are not using the Workspace, install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

6. Submit!


## Data flow
Dưới đây là giải thích về **data flow** (luồng dữ liệu) của chương trình dựa trên cấu trúc và các thành phần đã được cung cấp:

---

### **1. Khởi tạo chương trình**
- **`main()`**:
  - Chương trình bắt đầu từ hàm `main()`.
  - Trong `main()`, các đối tượng chính được khởi tạo:
    - **`System`**: Quản lý toàn bộ thông tin hệ thống.
    - **`NCursesDisplay`**: Hiển thị thông tin hệ thống lên giao diện dòng lệnh.
  - `NCursesDisplay::Display()` được gọi để bắt đầu hiển thị thông tin.

---

### **2. Lấy dữ liệu từ hệ thống**
- **`System`**:
  - Lớp `System` chịu trách nhiệm lấy dữ liệu từ hệ thống thông qua `LinuxParser`.
  - Các phương thức trong `System` gọi các hàm tương ứng trong `LinuxParser` để lấy thông tin:
    - **CPU**: `LinuxParser::CpuUtilization()` trả về mức sử dụng CPU.
    - **Bộ nhớ**: `LinuxParser::MemoryUtilization()` trả về mức sử dụng bộ nhớ.
    - **Thời gian hoạt động**: `LinuxParser::UpTime()` trả về thời gian hoạt động của hệ thống.
    - **Tiến trình**:
      - `LinuxParser::Pids()` trả về danh sách các PID (Process IDs).
      - `LinuxParser::Command()`, `LinuxParser::Ram()`, `LinuxParser::User()`, v.v., trả về thông tin chi tiết của từng tiến trình.
    - **Hệ điều hành và kernel**:
      - `LinuxParser::OperatingSystem()` trả về tên hệ điều hành.
      - `LinuxParser::Kernel()` trả về phiên bản kernel.

---

### **3. Xử lý dữ liệu**
- **`Processor`**:
  - Lớp `Processor` xử lý thông tin liên quan đến CPU, như tính toán mức sử dụng CPU dựa trên dữ liệu từ `LinuxParser`.
- **`Process`**:
  - Mỗi tiến trình được đại diện bởi một đối tượng `Process`.
  - `Process` lấy thông tin chi tiết của từng tiến trình từ `LinuxParser` (như PID, RAM, CPU utilization, thời gian hoạt động, v.v.).
- **`System`**:
  - Lớp `System` tổng hợp dữ liệu từ `Processor`, danh sách `Process`, và các thông tin khác (bộ nhớ, thời gian hoạt động, v.v.).

---

### **4. Hiển thị dữ liệu**
- **`NCursesDisplay`**:
  - Lớp `NCursesDisplay` chịu trách nhiệm hiển thị dữ liệu lên giao diện dòng lệnh.
  - Gồm hai phần chính:
    1. **`DisplaySystem()`**:
       - Hiển thị thông tin tổng quan về hệ thống, như:
         - Tên hệ điều hành.
         - Phiên bản kernel.
         - Mức sử dụng CPU và bộ nhớ (dưới dạng thanh tiến trình).
         - Tổng số tiến trình và số tiến trình đang chạy.
         - Thời gian hoạt động của hệ thống.
    2. **`DisplayProcesses()`**:
       - Hiển thị danh sách các tiến trình, bao gồm:
         - PID.
         - Tên người dùng.
         - Mức sử dụng CPU.
         - RAM.
         - Thời gian hoạt động.
         - Lệnh được thực thi.
  - `NCursesDisplay` liên tục cập nhật dữ liệu từ `System` và hiển thị thông tin theo thời gian thực.

---

### **Tóm tắt luồng dữ liệu**
1. **Dữ liệu từ hệ thống**:
   - `LinuxParser` đọc dữ liệu từ các file hệ thống trong proc (ví dụ: stat, meminfo, `/proc/[PID]/status`, v.v.).
2. **Xử lý dữ liệu**:
   - `System` và các thành phần con (`Processor`, `Process`) xử lý và lưu trữ dữ liệu.
3. **Hiển thị dữ liệu**:
   - `NCursesDisplay` lấy dữ liệu từ `System` và hiển thị lên giao diện dòng lệnh.

---

### **Luồng dữ liệu trực quan**
1. **`LinuxParser`**:
   - Lấy dữ liệu từ proc.
2. **`System`**:
   - Gọi `LinuxParser` để lấy dữ liệu.
   - Quản lý CPU (`Processor`) và danh sách tiến trình (`Process`).
3. **`NCursesDisplay`**:
   - Gọi `System` để lấy dữ liệu.
   - Hiển thị thông tin hệ thống và tiến trình.

---

### **Ví dụ cụ thể**
- **Mức sử dụng CPU**:
  1. `NCursesDisplay` gọi `System::Cpu()`.
  2. `System::Cpu()` trả về đối tượng `Processor`.
  3. `Processor` gọi `LinuxParser::CpuUtilization()` để lấy dữ liệu từ stat.
  4. `Processor` tính toán mức sử dụng CPU và trả về kết quả.
  5. `NCursesDisplay` hiển thị mức sử dụng CPU dưới dạng thanh tiến trình.

- **Danh sách tiến trình**:
  1. `NCursesDisplay` gọi `System::Processes()`.
  2. `System::Processes()` trả về danh sách các đối tượng `Process`.
  3. Mỗi `Process` gọi `LinuxParser` để lấy thông tin chi tiết (PID, RAM, CPU, v.v.).
  4. `NCursesDisplay` hiển thị danh sách tiến trình.

---

### **Kết luận**
Luồng dữ liệu của chương trình được thiết kế theo mô hình phân lớp:
- **`LinuxParser`**: Lấy dữ liệu từ hệ thống.
- **`System`**: Quản lý và xử lý dữ liệu.
- **`NCursesDisplay`**: Hiển thị dữ liệu lên giao diện dòng lệnh.

Cách tổ chức này giúp chương trình dễ bảo trì, mở rộng và tái sử dụng.