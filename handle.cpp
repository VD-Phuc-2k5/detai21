#include "index.h"

#define FILE_FORMAT "%[^#]#"
#define NEW_LINE "%[^\n]\n"
#define DATE_FORMAT "%d/%d/%d#"

#define find(queue, type, value, flag) do {\
	Node* head = queue.getHead();\
	while(head != NULL) {\
		if (!strcmp(((Don_thuoc*) head->data)->info[type], value)) {\
			printPrescription((Don_thuoc*) head->data);\
			flag = true;\
		}\
		head = head->next;\
	}\
} while(0)

#define TIM_DON_THUOC(queue, name, type, noti) do {\
	char value[MAX_SIZE_STRING];\
	bool flag = false;\
	printf("\nNhap %s thuoc ban muon tim: ", name);\
	fflush(stdin);\
	gets(value);\
	find(queue, type, value, flag);\
	if (!flag) {showNotification(noti);}\
} while(0)

#define TIM_THEO_HOA_DON(Prescription_Queue, BiLL_Queue) do {\
	char temp[MAX_SIZE_STRING];\
	bool flag = false;\
	printf("\nNhap ma hoa don cua don thuoc ban muon tim: ");\
	fflush(stdin); gets(temp);\
	Node* head = BiLL_Queue.getHead();\
	while (head != NULL) {\
		if (!strcmp(((Hoa_Don*) head->data)->info[maHoaDon], temp)) break;\
		head = head->next;\
	}\
	if (head == nullptr)\
		showNotification("\nMa hoa don khong ton tai trong danh sach hien tai!\n");\
	else {\
		strcpy(temp, ((Hoa_Don*) head->data)->info[maDonThuoc]);\
		find(Prescription_Queue, maThuoc, temp, flag);\
		showNotification("\nHay load them cac hoa don de tim kiem chinh xac hon!");\
	}\
} while(0)

const char* menuCase[] = {
	"0. Thoat",
	"1. Khoi tao danh sach don thuoc",
	"2. Lap hoa don",
	"3. Tim kiem thuoc",
	"4. In danh sach hoa don",
	"5. Tim thong tin don thuoc",
	"6. In ra thong tin cac loai thuoc ton so luong > 100",
	"7. Thong ke nhung don thuoc con han su dung duoi 30 ngay",
	"8. Xoa nhung thuoc da het han su dung hoac nhap lieu sai",
	"1. Lap hoa don nhap", "2. Lap hoa don xuat",
	"1. Tim theo ten thuoc", "2. Tim theo ma thuoc", "3. Tim theo hoa don",
	"1. Tim theo hoa don nhap", "2. Tim theo hoa don xuat",
	"1. In danh sach hoa don nhap", "2. In danh sach hoa don xuat",
	"1. Thuoc ban nhieu nhat", "2. Thuoc ban it nhat", "3. Thuoc co thoi gian su dung duoi 6 thang (tinh den ngay tim)",
};
// NHOM HAM THONG BAO  :
void showNotification(const char* notification) { 
	puts(notification);
	system("pause>0");
}
// NHOM HAM DOC FILE   :
void* readfile(FILE* file, const char* fileName) {
	if (file == NULL)
		showNotification("file khong ton tai\n");
	else {
		if (!strcmp(fileName, "thuoc.txt"))      {return getPrescription(file);}
		if (!strcmp(fileName, "hoadonxuat.txt")) {return getExportingBill(file);}
		if (!strcmp(fileName, "hoadonnhap.txt")) {return getImportingBill(file);}
	}
	return nullptr;
}

// NHOM HAM LAY DU LIEU:
// + HOA DON - DON THUOC
void getProperties(FILE* file, void* variable, const char* format, int pType, int vType) {
	char string[MAX_SIZE_STRING] = "";
    fscanf(file, format, string);
	if (vType == hoaDon)
		((Hoa_Don*)variable)->info[pType] = strdup(string);
	if (vType == donThuoc)
		((Don_thuoc*)variable)->info[pType] = strdup(string);
}
void* getPrescription(FILE* file) {
	Don_thuoc* value = new Don_thuoc;
	getProperties(file, value, FILE_FORMAT, maThuoc, donThuoc);
	getProperties(file, value, FILE_FORMAT, tenThuoc, donThuoc);
	getProperties(file, value, FILE_FORMAT, thanhPhanChinh, donThuoc);
	getProperties(file, value, FILE_FORMAT, congDung, donThuoc);
	getProperties(file, value, NEW_LINE, ghiChuCuaDonThuoc, donThuoc);
	return value;
}
Hoa_Don getBill(FILE* file) {
	Hoa_Don* value = new Hoa_Don;
	getProperties(file, value, FILE_FORMAT, maHoaDon, hoaDon);
	getProperties(file, value, FILE_FORMAT, ghiChuCuaHoaDon, hoaDon);
	getProperties(file, value, FILE_FORMAT, maDonThuoc, hoaDon);
	getProperties(file, value, FILE_FORMAT, soLo, hoaDon);
	getProperties(file, value, FILE_FORMAT, soLuong, hoaDon);
	getDate(file, DATE_FORMAT, value->date);
	return *value;
}
void* getExportingBill(FILE* file) {
	Hoa_Don_Xuat* value = new Hoa_Don_Xuat;
	value->info = getBill(file);
	fscanf(file, NEW_LINE, value->nguoi_mua);
	return value;
}
void* getImportingBill(FILE* file) {
	Hoa_Don_Nhap* value = new Hoa_Don_Nhap;
	value->info = getBill(file);
	getDate(file, DATE_FORMAT, value->dates[Ngay_sx]);
	getDate(file, DATE_FORMAT, value->dates[Ngay_het_han]);
	fscanf(file, NEW_LINE, value->Congty_sx);
	return value;
}
// + NGAY - THANG - NAM
void getDate(FILE* file, const char* format, Date& variable) {
	int d, m, y;
	fscanf(file, format, &d, &m, &y);
	variable = Date(d, m, y);
}
Date getCurrentDate() {
	Date start;
	time_t now;
    tm *currentTime;
    time(&now);
    currentTime = localtime(&now);
    start.day = currentTime->tm_mday;
    start.month = currentTime->tm_mon + 1; 
	start.year = currentTime->tm_year + 1900;
	return start;
}
// + MENU
void getChoice(int &choosen, int minCase, int maxCase) {
	do {
		printf("\nNhap lua chon: ");
		scanf("%d", &choosen);
	} while (choosen < minCase || choosen > maxCase);
}
void* getMaxOrMin(const char* type, Queue EXP_BiLL_Queue) {
	Hoa_Don_Xuat *max, *min;
	Node* head = EXP_BiLL_Queue.getHead();
	max = min = (Hoa_Don_Xuat*) head->data;
	while (head != NULL) {
		max = atoi(max->info.info[soLuong]) < atoi(((Hoa_Don_Xuat*) head->data)->info.info[soLuong]) ? (Hoa_Don_Xuat*) head->data : max;
		min = atoi(min->info.info[soLuong]) > atoi(((Hoa_Don_Xuat*) head->data)->info.info[soLuong]) ? (Hoa_Don_Xuat*) head->data : min;
		head = head->next;
	}
	if (!strcmp(type, "min")) return min;
	return max;
}

// NHOM HAM XUAT DU LIEU:
void printPrescription(Don_thuoc* value) {
	if (value == nullptr) return;
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n||%-6s||%-18s||%-18s||%-40s||%-60s||\n", value->info[maThuoc], 
							   value->info[tenThuoc], 
							   value->info[thanhPhanChinh],
							   value->info[congDung], 
							   value->info[ghiChuCuaDonThuoc]);
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n\n");
}
void printBill(Hoa_Don value, const char* string) {
	printf("\n= So hoa don %s    : %s %-122s =", string, value.info[maHoaDon], " ");
	printf("\n= Ngay %s          : %02d / %02d / %04d %-114s =", string, value.date.day,
									     value.date.month, 
								             value.date.year, " ");
	printf("\n= Ghi chu            : %-129s =", value.info[ghiChuCuaHoaDon]);
	printf("\n= Ma thuoc           : %-129s =", value.info[maDonThuoc]);
	printf("\n= So lo              : %-129s =", value.info[soLo]);
	printf("\n= So luong xuat      : %-129s =", value.info[soLuong]);
}
void printExportingBill(Hoa_Don_Xuat* value) {
	if (value == nullptr) return;
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printBill(value->info, "xuat");
	printf("\n= Nguoi mua          : %-129s =\n", value->nguoi_mua); 
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n\n");
}
void printImportingBill(Hoa_Don_Nhap* value) {
	if (value == nullptr) return;
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printBill(value->info, "nhap");
	printf("\n= Ngay san xuat      : %02d / %02d / %04d %-114s =", value->dates[Ngay_sx].day,
								value->dates[Ngay_sx].month, 
								value->dates[Ngay_sx].year, " ");
	printf("\n= Ngay het han       : %02d / %02d / %04d %-114s =", value->dates[Ngay_het_han].day,
								  value->dates[Ngay_het_han].month, 
								  value->dates[Ngay_het_han].year, " ");
	printf("\n= Cong ty san xuat   : %-129s =\n", value->Congty_sx);
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n\n");
}
// NHOM HAM KHAC       :
int daysInMonth(int month, int year) {
    int days[] = {31, 28 + (year % 4 == 0 && year % 100 != 0 || year % 400 == 0), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return days[month - 1];
}
bool isValidDate(Date date) {
    if (date.year < 0 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > daysInMonth(date.month, date.year))
        return false;
    return true;
}
bool checkExpiry(Date end) {
	Date start = getCurrentDate();
	if (end.year < start.year || (end.year == start.year && end.month < start.month) || 
	   (end.year == start.year && end.month == start.month && end.day < start.day))
		return true;
	return false;
}
int  checkMonth(Date end, int numbers, int type) { 
	Date start = getCurrentDate();
	if (type != 0) { Date temp = start; start = end; end = temp;}
	if (!isValidDate(end) || checkExpiry(end)) return -1;
	int days = 0;
	while (!(start.day == end.day && start.month == end.month && start.year == end.year)) {
        ++days;
		++start.day;
        if (start.day > daysInMonth(start.month, start.year)) {
            start.day = 1;
            ++start.month;
            if (start.month > 12) {
                start.month = 1;
                ++start.year;
            }
        }
    }
	return days < numbers ? 1 : 0;
}
//HANG DOI:.........................................................................................
void Queue::init() {Head = Tail = nullptr;}
bool Queue::isEmpty() {return Head == nullptr;}
void Queue::push(void* value) {
	if (value == nullptr) {showNotification("Du lieu khong hop le\n");}
	Node *newNode = new Node(value);
	isEmpty() ? Head = newNode : Tail->next = newNode;
	Tail = newNode;
}
void Queue::create(int size, FILE* file, const char* fileName) {
	void* value = nullptr;
	while (size--) {
		value = readfile(file, fileName);
		push(value);
	}
}
void* Queue::pop() {
	if (isEmpty()) {return nullptr;}
	Node* temp = Head;
	void* value = temp->data;
	Head = Head->next;
	if (isEmpty()) Tail = nullptr;
	delete temp;
	return value;
}
void Queue::print(const char* type) {
	int count = 0;
	if (Head == nullptr) 
		showNotification("\nHang doi rong");
	Node* current = Head;
	while (current != nullptr) {
		printf("\n - %s %02d:\n", type, ++count);
		if (!strcmp(type, "DON THUOC"))    {printPrescription ((Don_thuoc*)    current->data);}
		if (!strcmp(type, "HOA DON NHAP")) {printImportingBill((Hoa_Don_Nhap*) current->data);}
		if (!strcmp(type, "HOA DON XUAT")) {printExportingBill((Hoa_Don_Xuat*) current->data);}
		current =  current->next;
	}
}
Node* Queue::getHead()  {return Head;}
// DANH SACH LIEN KIET:.............................................................................
void* List::deleteAfter(Node* current) {
	if (current->next == Tail) Tail = current;
	void* value = current->next->data;
	Node* next_next = current->next->next;
	delete current->next;
	current->next = next_next;
	return value;
}
// CHUC NANG CHINH CUA CHUONG TRINH
void Menu(int start, int end) {
	printf("MENU:\n");
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n||\t%-140s\t||\n", menuCase[0]); 
	for (int i = start; i <= end; i++) {
		printf("\n||\t%-140s\t||\n", menuCase[i]); 
	}
	for (int i = 0; i < MENU_WIDTH; i++) {printf("=");}
	printf("\n");
}
// case 1
void KHOI_TAO_DS_DON_THUOC(List& Prescription_List, FILE* Don_thuoc) {
	if (!Prescription_List.isEmpty()) {
		showNotification("\nDanh sach don thuoc da ton tai!");
		return;
	}
	int size;
	fscanf(Don_thuoc, "%d\n", &size);
	Prescription_List.create(size, Don_thuoc, FILE_DON_THUOC);
	showNotification("\nDa tao danh sach don thuoc!");
}
// case 2
void LAP_HOA_DON(Queue& IMP_BiLL_Queue, Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Nhap, FILE* Hoa_Don_Xuat) {
	int choice;
	do {
		system("cls");
		Menu(9, 10);
		getChoice(choice, 0, 2);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: LAP_HOA_DON_NHAP(IMP_BiLL_Queue, Hoa_Don_Nhap); break;
			case 2: LAP_HOA_DON_XUAT(EXP_BiLL_Queue, Hoa_Don_Xuat); break;
		}
	} while (choice);
}
void LAP_HOA_DON_NHAP(Queue& IMP_BiLL_Queue, FILE* Hoa_Don_Nhap) {
	if (!feof(Hoa_Don_Nhap)) {
		IMP_BiLL_Queue.create(SIZE, Hoa_Don_Nhap, FILE_HOA_DON_NHAP);
		printf("\nDa Load them %d hoa don nhap thanh cong!", SIZE);
		system("pause>0");
	}
	else 
		showNotification("\nDa load toan bo du lieu tu file vao hang doi!");
}	
void LAP_HOA_DON_XUAT(Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Xuat) {
	if (!feof(Hoa_Don_Xuat)) {
		EXP_BiLL_Queue.create(SIZE, Hoa_Don_Xuat, FILE_HOA_DON_XUAT);
		printf("\nDa Load them %d hoa don nhap thanh cong!", SIZE);
		system("pause>0");
	}
	else 
		showNotification("\nDa load toan bo du lieu tu file vao hang doi!");
}
// case 3
void TIM_KIEM_DON_THUOC(List Prescription_List, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	if (Prescription_List.isEmpty() || IMP_BiLL_Queue.isEmpty() || EXP_BiLL_Queue.isEmpty()) {
		showNotification("\nTon Tai du lieu chua duoc khoi tao!");
		return; 
	}
	int choice;
	char* noti = "\nResult: Khong co don thuoc ban tim trong danh sach!";
	do {
		system("cls");
		Menu(11, 13);
		getChoice(choice, 0, 3);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: TIM_DON_THUOC(Prescription_List, "ten", tenThuoc, noti); break;
			case 2: TIM_DON_THUOC(Prescription_List, "ma so", maThuoc, noti); break;
			case 3: TIM_DON_THUOC_THEO_HOA_DON(Prescription_List, IMP_BiLL_Queue, EXP_BiLL_Queue); break;
		}
		system("pause>0");
	} while (choice);
}
void TIM_DON_THUOC_THEO_HOA_DON(List Prescription_List, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	int choice;
	do {
		system("cls");
		Menu(14, 15);
		getChoice(choice, 0, 2);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: TIM_THEO_HOA_DON(Prescription_List, IMP_BiLL_Queue); break;
			case 2: TIM_THEO_HOA_DON(Prescription_List, EXP_BiLL_Queue); break;
		}
		system("pause>0");
	} while (choice);
}
// case 4
void IN_DANH_SACH_HOA_DON(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	int choice;
	do {
		system("cls");
		Menu(16, 17);
		getChoice(choice, 0, 2);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: IMP_BiLL_Queue.print("HOA DON NHAP"); break;
			case 2: EXP_BiLL_Queue.print("HOA DON XUAT"); break;
		}
		system("pause>0");
	} while (choice);
}
// case 5
void TIM_THONG_TIN_DON_THUOC(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	int choice;
	do {
		system("cls");
		Menu(18, 20);
		getChoice(choice, 0, 3);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: THONG_TIN_DON_THUOC_BAN_NHIEU_NHAT_IT_NHAT(EXP_BiLL_Queue, "max"); break;
			case 2: THONG_TIN_DON_THUOC_BAN_NHIEU_NHAT_IT_NHAT(EXP_BiLL_Queue, "min"); break;
			case 3: THONG_TIN_DON_THUOC_CO_THOI_GIAN_SD_DUOI_6_THANG(IMP_BiLL_Queue); break;
		}
		system("pause>0");
	} while (choice);
}
void THONG_TIN_DON_THUOC_BAN_NHIEU_NHAT_IT_NHAT(Queue EXP_BiLL_Queue, const char* type) {
	if (EXP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	Hoa_Don_Xuat value = *((Hoa_Don_Xuat*) getMaxOrMin(type, EXP_BiLL_Queue));
	Node* head = EXP_BiLL_Queue.getHead();
	while(head != NULL) {
		if (atoi(value.info.info[soLuong]) == atoi(((Hoa_Don_Xuat*)head->data)->info.info[soLuong]))
			printExportingBill((Hoa_Don_Xuat*) head->data);
		head = head->next;
	}
}
void THONG_TIN_DON_THUOC_CO_THOI_GIAN_SD_DUOI_6_THANG(Queue IMP_BiLL_Queue) {
	if (IMP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* head = IMP_BiLL_Queue.getHead();
	while (head != nullptr) {
		if (checkMonth(((Hoa_Don_Nhap*) head->data)->dates[Ngay_sx], 180, 1) == 1) {
			flag = true;
			printImportingBill((Hoa_Don_Nhap*) head->data);
		}
		head = head->next;
	}
	if (!flag) showNotification("\n Khong co don thuoc phu hop voi yeu cau!");
}
// case 6
void IN_RA_THONG_TIN_CAC_DON_THUOC_TON_SLG_LON_HON_N(int number, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	if (IMP_BiLL_Queue.isEmpty() || EXP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* h1 = IMP_BiLL_Queue.getHead();
	Node* h2 = EXP_BiLL_Queue.getHead();
	while(h1 != NULL && h2 != NULL)
	{
		if (atoi(((Hoa_Don_Nhap*) h1->data)->info.info[soLuong]) - atoi(((Hoa_Don_Xuat*) h2->data)->info.info[soLuong]) > number) {
			flag = true;
			printf("\nResult:\n");
			printImportingBill((Hoa_Don_Nhap*) h1->data);
			printExportingBill((Hoa_Don_Xuat*) h2->data);
		}
		h1 = h1->next;
		h2 = h2->next;
	}
	if (!flag) showNotification("\nKhong co thong tin phu hop");
}
// case 7
void THONG_KE_THUOC_CO_THOI_HAN_SU_DUNG_DUOI_30_NGAY(Queue IMP_BiLL_Queue) {
	if (IMP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* head = IMP_BiLL_Queue.getHead();
	while (head != NULL) {
		if (checkMonth(((Hoa_Don_Nhap*) head->data)->dates[Ngay_het_han], 30, 0) == 1) {
			flag = true;
			printImportingBill((Hoa_Don_Nhap*) head->data);
		}
		head = head->next;
	}
	if (!flag) showNotification("\nKhong co don thuoc nao co han su dung duoi 30 ngay");
}
// case 8
void XOA_DON_THUOC(List& Prescription_List, Queue& IMP_BiLL_Queue) {
	if (Prescription_List.isEmpty() || IMP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	Node* h1 = Prescription_List.getHead(), *h2 = IMP_BiLL_Queue.getHead();
	Node* prev = nullptr;
	Date key;
	while (h2 != nullptr) {
		key = ((Hoa_Don_Nhap*) h2->data)->dates[Ngay_het_han];
		if (checkExpiry(key)) {
			if (!strcmp(((Hoa_Don_Nhap*) h2->data)->info.info[maDonThuoc], ((Don_thuoc*) h1->data)->info[maThuoc])) 
			{
				prev == nullptr ? printPrescription((Don_thuoc*) Prescription_List.pop()) :
					              printPrescription((Don_thuoc*) Prescription_List.deleteAfter(prev));
				printImportingBill((Hoa_Don_Nhap*) h2->data);
				h1 = Prescription_List.getHead();
				h2 = h2->next;
			}
			else { prev = h1; h1 = h1->next; }
		}
		else h2 = h2->next;
	}
	while(IMP_BiLL_Queue.pop());
	printf("\nDa xoa cac don thuoc het han khoi danh muc thuoc!");
}
