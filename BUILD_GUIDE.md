# Hướng Dẫn Build GnuBox S60v3 FP2 Trên Máy Tính Này

Tài liệu này hướng dẫn chi tiết cách thiết lập môi trường, biên dịch mã nguồn C++ Symbian và đóng gói file cài đặt `.sisx` cho **GnuBox (Vetera-Bridge Edition)** trực tiếp trên máy tính Windows này.

---

## 1. Môi trường & Công cụ đã cài sẵn trên máy

Máy tính này đã được cấu hình đầy đủ toàn bộ bộ công cụ lập trình Symbian OS 9.3 (S60 3rd Edition FP2):

| Thành phần | Đường dẫn thực tế trên máy | Đường dẫn rút gọn DOS (8.3) | Ghi chú |
| :--- | :--- | :--- | :--- |
| **Symbian SDK** | `C:\S60\devices\S60_3rd_FP2_SDK_v1.1\` | - | S60 3rd Edition Feature Pack 2 SDK v1.1 |
| **CSL ARM Toolchain** | `C:\Program Files (x86)\CSL Arm Toolchain\` | `C:\PROGRA~2\CSLARM~1\` | Trình biên dịch GCCE 3.4.3 cho CPU ARM |
| **ActivePerl** | `C:\Perl\bin\perl.exe` | - | Perl 5.6.1 dùng cho các công cụ Symbian |
| **Symbian Tools** | `C:\Program Files (x86)\Common Files\Symbian\Tools\` | `C:\PROGRA~2\COMMON~1\Symbian\tools\` | Chứa `devices.exe`, `epoc.exe` |
| **SDK Tools** | `C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools\` | - | Chứa `bldmake`, `abld`, `makesis`, `signsis` |
| **Git CLI** | `C:\Program Files\Git\cmd\git.exe` | - | Dùng để push/pull mã nguồn |

### Vị trí thư mục dự án:
- **Thư mục làm việc & build:** `C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera`
- **Thư mục Git repository:** `C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera-git`

---

## 2. Cách Build tự động 1-Click (Khuyên dùng)

Cách nhanh nhất và không lo nhầm lẫn biến môi trường là sử dụng script tự động `build_s60v3.bat`.

### Thao tác bằng Command Prompt (`cmd.exe`):
```cmd
cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\group
build_s60v3.bat
```

*(Hoặc nếu đang làm việc trong thư mục git: `cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera-git\group && build_s60v3.bat`)*

### Thao tác bằng PowerShell:
```powershell
cmd /c "cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\group && build_s60v3.bat"
```

Khi màn hình hiện dòng thông báo:
```text
=== BUILD AND PACKAGING COMPLETE ===
```
Nghĩa là quá trình biên dịch và ký số hoàn toàn thành công.

---

## 3. Quy trình Build từng bước chi tiết

Nếu muốn chạy từng lệnh thủ công để debug lỗi hoặc xem chi tiết, hãy mở `cmd.exe` và làm theo các bước sau:

### Bước 1: Khởi tạo biến môi trường
Mở `cmd.exe` và chạy:
```cmd
set EPOCROOT=\S60\devices\S60_3rd_FP2_SDK_v1.1\
set GCC_EXEC_PREFIX=
set COMPILER_PATH=
set PATH=C:\PROGRA~2\CSLARM~1\bin;C:\Perl\bin;C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools;C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\gcc\bin;C:\PROGRA~2\COMMON~1\Symbian\tools;C:\Windows\system32;C:\Windows
```

### Bước 2: Tạo Makefile từ file cấu hình dự án
Chuyển vào thư mục `group`:
```cmd
cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\group
bldmake bldfiles
abld makefile gcce
```
> **Giải thích:**
> - `bldmake bldfiles`: Đọc `bld.inf` để tạo các file build trung gian.
> - `abld makefile gcce`: Đọc `gnubox.mmp` để sinh Makefile chuẩn cho GCCE.

### Bước 3: Cấu hình Compiler Path và Biên dịch Resource
```cmd
set GCC_EXEC_PREFIX=C:\PROGRA~2\CSLARM~1\lib\gcc\
set COMPILER_PATH=C:\PROGRA~2\CSLARM~1\libexec\gcc\arm-none-symbianelf\3.4.3;C:\PROGRA~2\CSLARM~1\arm-none-symbianelf\bin;C:\PROGRA~2\CSLARM~1\bin
set PATH=C:\PROGRA~2\CSLARM~1\bin;C:\PROGRA~2\CSLARM~1\libexec\gcc\arm-none-symbianelf\3.4.3;C:\PROGRA~2\CSLARM~1\arm-none-symbianelf\bin;%PATH%

abld resource gcce urel
```
> **Giải thích:** Biên dịch các file `.rss` trong thư mục `data/` (`gnubox.rss`, `gnubox_reg.rss`) thành binary resource `.rsc` và file header định danh `.rsg`.

### Bước 4: Biên dịch mã nguồn C++ sang file thực thi ARM
```cmd
abld target gcce urel
abld final gcce urel
```
> **Giải thích:**
> - `abld target gcce urel`: Trình biên dịch `arm-none-symbianelf-g++` sẽ biên dịch toàn bộ các file `.cpp` trong `src/` thành file mã máy:
>   `C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\release\gcce\urel\gnubox.exe`
> - `abld final gcce urel`: Đưa các resource và dữ liệu liên kết vào vị trí hoàn thiện.

### Bước 5: Đóng gói và Ký số (Package & Sign)
Chuyển sang thư mục `sis`:
```cmd
cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\sis

copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\release\gcce\urel\gnubox.exe" "gnubox.exe"
copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\data\z\resource\apps\gnubox.rsc" "gnubox.rsc"
copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\data\z\private\10003a3f\apps\gnubox_reg.rsc" "gnubox_reg.rsc"

"C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools\makesis.exe" gnubox_s60v3_vetera.pkg gnubox_s60v3_vetera.sis
"C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools\signsis.exe" -s gnubox_s60v3_vetera.sis gnubox_s60v3_vetera.sisx selfsign.cer selfsign.key
```
> **Giải thích:**
> - `makesis`: Đóng gói file binary `.exe`, resource `.rsc`, và thông tin đăng ký ứng dụng thành gói cài đặt thô `.sis`.
> - `signsis`: Sử dụng chứng chỉ `selfsign.cer` và khóa riêng `selfsign.key` để tạo thành gói cài đặt `.sisx` đã ký, tương thích với cơ chế kiểm tra Symbian Signed trên S60 3rd Edition FP2.

---

## 4. Dọn dẹp bản build (Clean Build) khi cần

Nếu bạn sửa đổi file header `.h` lớn, sửa cấu trúc `gnubox.mmp` hoặc gặp lỗi xung đột cache giữa các lần build, hãy chạy các lệnh sau:

```cmd
cd /d C:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\group
abld clean gcce urel
bldmake clean
```
Sau đó chạy lại `build_s60v3.bat` để build hoàn toàn từ đầu.

---

## 5. Kết quả đầu ra (Output Files)

Sau khi quá trình build hoàn tất, các file sau được tạo ra:

1. **`gnubox_s60v3_vetera.sisx`** *(~54 KB)*:
   - File cài đặt chính thức (đã ký số).
   - Nằm tại: `sis\gnubox_s60v3_vetera.sisx` và được copy ra thư mục gốc dự án.
   - Chép trực tiếp file này vào thẻ nhớ hoặc qua Bluetooth vào điện thoại (Nokia E5-00, C5-00, E71, E72, N95...) để cài đặt.
2. **`gnubox.exe`** *(~52 KB)*:
   - File binary thực thi ARM. Nằm tại `epoc32\release\gcce\urel\gnubox.exe`.

---

## 6. Lưu ý khi cài đặt lên điện thoại Nokia S60v3

Vì đây là gói phần mềm tự ký số (Self-signed):
1. **Bật cho phép cài đặt:** Trên điện thoại, vào `Menu -> Cài đặt -> Cài đặt ứng dụng -> Quản lý ứng dụng`:
   - **Cài đặt phần mềm** (*Software installation*): Chọn **Tất cả** (*All*).
   - **Kiểm tra c.chỉ trực tuyến** (*Online certificate check*): Chọn **Tắt** (*Off*).
2. **Lỗi chứng chỉ hết hạn:** Nếu điện thoại báo *"Chứng chỉ hết hạn"* (*Certificate expired*), hãy chỉnh ngày giờ của điện thoại về khoảng năm 2010–2015 rồi tiến hành cài đặt, cài xong có thể chỉnh lại ngày giờ hiện tại bình thường.
