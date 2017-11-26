### Thông Tin Cá Nhân ###
#### Họ tên: ####
 Nguyễn Đăng Quang
#### MSSV: ####
 1512432 
#### Mail: ####
 nguyendangquangkt@gmail.com

### Các Chức năng Đã Làm Được ###
#### Các yêu cầu cơ bản ####
##### GDI+ #####
- Vẽ các đường thẳng (line), hình elippse, hình tròn, vuông, chữ nhật bằng hàm Draw* trong thư viện GDI+
- nếu vẽ hình chữ nhật và bấm phím shift sẽ chuyển sang hình vuông, và thả ra sẽ quay lại thành hình chữ nhật
- Nếu vẽ hình elippse và nhấn shift sẽ chuyển thành hình tròn, thả shift sẽ quay lại thành hình elippse
- Mặc định chưa chỉ định nét vẽ, chưa chỉ định màu vẽ, chưa chỉ định màu tô, loại chổi tô, các thuộc tính này được gán cố định

##### Ribbon #####
- Tạo menu file với các chức năng Save, Open, New, Exit bằng ribbon MenuApplication
- Tạo tab Shapes với các chức năng vẽ đường thẳng, hình chữ nhật/hình vuông, đường ellipse/đường tr
- Tạo Quick eccess toolbar với các label mặc định là save, new, open
#### các yêu cầu nâng cao ####
- lưu và load hình ở dạng binary
- Bọc tất cả các đối tượng vẽ vào các lớp model. Sử dụng đa xạ (polymorphism) để cài đặt việc quản lý các đối tượng và vẽ hình. Sử dụng mẫu thiết kế prototypes để tạo ra hàng mẫu nhằm vẽ ở chế độ xem trước (prev
-  Lưu và nạp các object (Lưu mảng các tọa độ ở dạng tập tin nhị phân)

### Các Luồng Sự kiện chính ( Main Flow)  ### 
- Mở chương trình lên sẽ có màn hình vẽ và tab Shapes thể hiện các icon ảnh hỉnh vẽ từng đối tượng đường thẳng, đường ellipse,chữ nhật, chọn các icon tương ứng tren tab để set kiểu vẽ
- khi vẽ hình chữ nhật nếu giữ phím Shift sẽ chuyển sang vẽ hình vuông và khi đang vẽ đường ellipse nếu giữ phím shift sẽ chuyển sang vẽ đường tròn
- tab file sẽ có các icon save,open,new với các chức năng tương ứng lưu, mở tập tin binary (ảnh), làm mới màn hình vẽ
### Các Luồng Sự Kiện Phụ (add flow) ###
- Chưa thể export hình ra file ảnh định dạng png
### Link tới RePo trên Bitbucket ###
https://bitbucket.org/QuangDangFIT/paint-with-gdi-and-ribbon
### Link Youtube ###
https://goo.gl/4K9pGw