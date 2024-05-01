#include "index.h"
//=====================================================================//
#define getString(file, value, format, index) do {\
	char string[MAX_SIZE];\
    fscanf(file, format, string);\
    value->info[index] = strdup(string);\
} while(0)
//=====================================================================//
#define getInfo(file, value, format, size) do {\
	for(int i = 0; i < size; i++)\
		getString(file, value, format, i);\
} while(0)
//=====================================================================//
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
//=====================================================================//
#define TIM_DON_THUOC(queue, name, type, noti) do {\
	char value[MAX_SIZE];\
	bool flag = false;\
	printf("\nNhap %s thuoc ban muon tim: ", name);\
	fflush(stdin);\
	gets(value);\
	find(queue, type, value, flag);\
	if (!flag) {showNotification(noti);}\
} while(0)
//=====================================================================//
#define TIM_THEO_HOA_DON(Prescription_Queue, BiLL_Queue) do {\
	bool flag = false;\
	char temp[MAX_SIZE];\
	Node* head = BiLL_Queue.getHead();\
	printf("\nNhap ma hoa don cua don thuoc ban muon tim: ");\
	fflush(stdin);\
	gets(temp);\
	while (head != NULL) {\
		if (!strcmp(((Hoa_Don*) head->data)->info[0], temp)) {flag = true; break;}\
		head = head->next;\
	}\
	if (!flag) {showNotification("\nMa hoa don khong ton tai trong danh sach hien tai!\n");}\
	else {\
		strcpy(temp, ((Hoa_Don*) head->data)->info[2]);\
		find(Prescription_Queue, 0, temp, flag);\
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

void showNotification(const char* notification) { 
	puts(notification);
	system("pause>0");
}
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

// Don thuoc // 
void* getPrescription(FILE* file) {
	Don_thuoc* value = new Don_thuoc();
	getInfo(file, value, "%[^#]#", 4);
	getString(file, value, "%[^\n]\n", 4);
	return value;
}
void printPrescription(Don_thuoc* value) {
	if (value == nullptr) return;
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printf("\n||%-6s||%-18s||%-18s||%-40s||%-60s||\n", value->info[0], value->info[1], value->info[2], value->info[3], value->info[4]);
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printf("\n");
}

// Hoa don //
void* getBill(FILE* file) {
	Hoa_Don* value = new Hoa_Don();
	getInfo(file, value, "%[^#]#", 5);
	fscanf(file, "%d/%d/%d#", &value->date.day, &value->date.month, &value->date.year);
	return value;
}
void printBill(Hoa_Don value, const char* string) {
	printf("\n= So hoa don %s    : %s", string, value.info[0]);
	printf("\n= Ngay %s          : %02d / %02d / %04d", string, value.date.day, value.date.month, value.date.year);
	printf("\n= Ghi chu            : "); puts(value.info[1]);
	printf("= Ma thuoc           : "); puts(value.info[2]);
	printf("= So lo              : "); puts(value.info[3]);
	printf("= So luong xuat      : "); puts(value.info[4]);
}

// Hoa don xuat //
void* getExportingBill(FILE* file) {
	Hoa_Don_Xuat* value = new Hoa_Don_Xuat();
	value->info = *((Hoa_Don*) getBill(file));
	fscanf(file, "%[^\n]\n", value->nguoi_mua);
	return value;
}
void printExportingBill(Hoa_Don_Xuat* value) {
	if (value == nullptr) return;
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printBill(value->info, "xuat");
	printf("= Nguoi mua          : "); puts(value->nguoi_mua);
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printf("\n");
}

// Hoa don nhap //
void* getImportingBill(FILE* file) {
	Hoa_Don_Nhap* value = new Hoa_Don_Nhap();
	value->info = *((Hoa_Don*)getBill(file));
	fscanf(file, "%d/%d/%d#", &value->Ngay_sx.day, &value->Ngay_sx.month, &value->Ngay_sx.year);
	fscanf(file, "%d/%d/%d#", &value->Ngay_het_han.day, &value->Ngay_het_han.month, &value->Ngay_het_han.year);
	fscanf(file, "%[^\n]\n", value->Congty_sx);
	return value;
}
void printImportingBill(Hoa_Don_Nhap* value) {
	if (value == nullptr) return;
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printBill(value->info, "nhap");
	printf("= Ngay san xuat      : %02d / %02d / %04d", value->Ngay_sx.day, value->Ngay_sx.month, value->Ngay_sx.year);
	printf("\n= Ngay het han       : %02d / %02d / %04d", value->Ngay_het_han.day, value->Ngay_het_han.month, value->Ngay_het_han.year);
	printf("\n= Cong ty san xuat   : "); puts(value->Congty_sx);
	for (int i = 0; i < WIDTH; i++) {printf("_");}
	printf("\n");
}

// Menu function
void Menu(int start, int end) {
	printf("MENU:\n");
	for (int i = 0; i < WIDTH; i++) {printf("=");}
	printf("\n||\t"); puts(menuCase[0]);
	for (int i = start; i <= end; i++) {
		printf("\n||\t"); 
		puts(menuCase[i]);
	}
	for (int i = 0; i < WIDTH; i++) {printf("=");}
	printf("\n");
}
void getChoice(int &choosen, int minCase, int maxCase) {
	do {
		printf("\nNhap lua chon: ");
		scanf("%d", &choosen);
	} while (choosen < minCase || choosen > maxCase);
}
void KHOI_TAO_DS_DON_THUOC(Queue& Prescription_Queue, FILE* Don_thuoc) {
	if (!Prescription_Queue.isEmpty()) {
		showNotification("\nDanh sach don thuoc da ton tai!");
		return;
	}
	int size;
	fscanf(Don_thuoc, "%d\n", &size);
	Prescription_Queue.createQueue(size, Don_thuoc, fileDonThuoc);
	showNotification("\nDa tao danh sach don thuoc!");
}
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
		IMP_BiLL_Queue.createQueue(SIZE, Hoa_Don_Nhap, fileHoaDonNhap);
		printf("\nDa Load them %d hoa don nhap thanh cong!", SIZE);
		system("pause>0");
	}
	else
		showNotification("\nDa load toan bo du lieu tu file vao hang doi!");
}	
void LAP_HOA_DON_XUAT(Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Xuat) {
	if (!feof(Hoa_Don_Xuat)) {
		EXP_BiLL_Queue.createQueue(SIZE, Hoa_Don_Xuat, fileHoaDonXuat);
		printf("\nDa Load them %d hoa don nhap thanh cong!", SIZE);
		system("pause>0");
	}
	else
		showNotification("\nDa load toan bo du lieu tu file vao hang doi!");
}
void TIM_KIEM_DON_THUOC(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	if (Prescription_Queue.isEmpty() || IMP_BiLL_Queue.isEmpty() || EXP_BiLL_Queue.isEmpty()) {
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
			case 1: TIM_DON_THUOC(Prescription_Queue, "ten", 1, noti); break;
			case 2: TIM_DON_THUOC(Prescription_Queue, "ma so", 0, noti); break;
			case 3: TIM_DON_THUOC_THEO_HOA_DON(Prescription_Queue, IMP_BiLL_Queue, EXP_BiLL_Queue); break;
		}
		system("pause>0");
	} while (choice);
}
void TIM_DON_THUOC_THEO_HOA_DON(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	int choice;
	do {
		system("cls");
		Menu(14, 15);
		getChoice(choice, 0, 2);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: TIM_THEO_HOA_DON(Prescription_Queue, IMP_BiLL_Queue); break;
			case 2: TIM_THEO_HOA_DON(Prescription_Queue, EXP_BiLL_Queue); break;
		}
		system("pause>0");
	} while (choice);
}
void IN_DANH_SACH_HOA_DON(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	int choice;
	do {
		system("cls");
		Menu(16, 17);
		getChoice(choice, 0, 2);
		switch (choice) {
			case 0: printf("\nBam mot phim bat ki de tiep tuc!"); break;
			case 1: IMP_BiLL_Queue.printQueue("hoa don nhap"); break;
			case 2: EXP_BiLL_Queue.printQueue("hoa don xuat"); break;
		}
		system("pause>0");
	} while (choice);
}
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
		if (atoi(value.info.info[4]) == atoi(((Hoa_Don_Xuat*)head->data)->info.info[4]))
			printExportingBill((Hoa_Don_Xuat*) head->data);
		head = head->next;
	}
}
void* getMaxOrMin(const char* type, Queue EXP_BiLL_Queue) {
	Hoa_Don_Xuat *max, *min;
	Node* head = EXP_BiLL_Queue.getHead();
	max = min = (Hoa_Don_Xuat*) head->data;
	while (head != NULL) {
		max = atoi(max->info.info[4]) < atoi(((Hoa_Don_Xuat*) head->data)->info.info[4]) ? (Hoa_Don_Xuat*) head->data : max;
		min = atoi(min->info.info[4]) > atoi(((Hoa_Don_Xuat*) head->data)->info.info[4]) ? (Hoa_Don_Xuat*) head->data : min;
		head = head->next;
	}
	if (!strcmp(type, "min")) return min;
	return max;
}
void THONG_TIN_DON_THUOC_CO_THOI_GIAN_SD_DUOI_6_THANG(Queue IMP_BiLL_Queue) {
	if (IMP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* head = IMP_BiLL_Queue.getHead();
	while (head != NULL) {
		if (checkMonth(((Hoa_Don_Nhap*) head->data)->Ngay_het_han, 180) == 1) {
			flag = true;
			printImportingBill((Hoa_Don_Nhap*) head->data);
		}
		head = head->next;
	}
	if (!flag) showNotification("\n Khong co don thuoc phu hop voi yeu cau!");
}
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
	if (end.year < start.year || (end.year == start.year && end.month < start.month) || (end.year == start.year && end.month == start.month && end.day < start.day))
		return true;
	return false;
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
int  checkMonth(Date end, int numbers) {
	Date start = getCurrentDate();
	int days = 0;
	if (!isValidDate(end) || checkExpiry(end)) return -1;
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
		if (atoi(((Hoa_Don_Nhap*) h1->data)->info.info[4]) - atoi(((Hoa_Don_Xuat*) h2->data)->info.info[4]) > number) {
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
void THONG_KE_THUOC_CO_THOI_HAN_SU_DUNG_DUOI_30_NGAY(Queue IMP_BiLL_Queue) {
	if (IMP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* head = IMP_BiLL_Queue.getHead();
	while (head != NULL) {
		if (checkMonth(((Hoa_Don_Nhap*) head->data)->Ngay_het_han, 30) == 1) {
			flag = true;
			printImportingBill((Hoa_Don_Nhap*) head->data);
		}
		head = head->next;
	}
	if (!flag) showNotification("\nKhong co don thuoc nao co han su dung duoi 30 ngay");
}
void XOA_DON_THUOC(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue) {
	if (Prescription_Queue.isEmpty() || IMP_BiLL_Queue.isEmpty() ||EXP_BiLL_Queue.isEmpty()) {
		showNotification("\nDanh sach chua duoc khoi tao!");
		return;
	}
	bool flag = false;
	Node* h1 = Prescription_Queue.getHead();
	Node* h2 = IMP_BiLL_Queue.getHead();
	Node* h3 =  EXP_BiLL_Queue.getHead();
	Date end = ((Hoa_Don_Nhap*) h2->data)->Ngay_het_han;
	if (!isValidDate(end) || checkExpiry(end)){
		printf("\nDON THUOC:\n");
		printPrescription((Don_thuoc*) Prescription_Queue.pop());
		printImportingBill((Hoa_Don_Nhap*)IMP_BiLL_Queue.pop());
		printExportingBill((Hoa_Don_Xuat*)EXP_BiLL_Queue.pop());
		flag = true;
	}
	while (h2->next != NULL) {
		end = ((Hoa_Don_Nhap*) h2->next->data)->Ngay_het_han;
		if (!isValidDate(end) || checkExpiry(end)) {
			printf("\nDON THUOC:\n");
			printPrescription((Don_thuoc*) Prescription_Queue.deleteAfter(h1));
			printImportingBill((Hoa_Don_Nhap*)IMP_BiLL_Queue.deleteAfter(h2));
			printExportingBill((Hoa_Don_Xuat*)EXP_BiLL_Queue.deleteAfter(h3));
			flag = true;
		}
		h1 = h1->next; h2 = h2->next; h3 = h3->next;
	}
	if (flag) showNotification("\nDa xoa cac don thuoc tren khoi danh sach hien tai!");
	else  showNotification("\nKhong co don thuoc nao can xoa!");
}