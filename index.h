#ifndef index_h
#define index_h

// define libraries //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// define macros
#define fileDonThuoc   "thuoc.txt"
#define fileHoaDonNhap "hoadonnhap.txt"
#define fileHoaDonXuat "hoadonxuat.txt"
#define MAX_SIZE 255
#define WIDTH 154
#define SIZE 10 // so phan tu load len trc

#pragma warning (disable : 4996)

// define structures
struct Date {
	short int day, month;
	int year;
};
struct Don_thuoc {
	char *info[5];
	Don_thuoc() {for(int i = 0; i < 5; i++) {info[i] = "";}}
};
struct Hoa_Don {
	Date date;
	char* info[5];
	Hoa_Don() {for(int i = 0; i < 5; i++) {info[i] = "";}}
};
struct Hoa_Don_Xuat {
	Hoa_Don info;
	char nguoi_mua[MAX_SIZE];
	Hoa_Don_Xuat() {for(int i = 0; i < 5; i++) {info.info[i] = "";}}
};
struct Hoa_Don_Nhap {
	Hoa_Don	info;
	Date Ngay_sx, Ngay_het_han;
	char Congty_sx[MAX_SIZE];
	Hoa_Don_Nhap() {for(int i = 0; i < 5; i++) {info.info[i] = "";}}
};
struct Node {
	void* data;
	Node* next;
	Node(void* newData) : data(newData), next(nullptr) {}
};

// define prototypes //
void showNotification(const char* notification);
void* readfile(FILE* file, const char* fileName);
// Don thuoc //
void* getPrescription(FILE* file);
void printPrescription(Don_thuoc* value);
// Hoa don //
void* getBill(FILE* file);
void printBill(Hoa_Don value, const char* string);
// Hoa don xuat //
void* getExportingBill(FILE* file);
void printExportingBill(Hoa_Don_Xuat* value);
// Hoa don nhap //
void* getImportingBill(FILE* file);
void printImportingBill(Hoa_Don_Nhap* value);

struct Queue {
	Node* Head, * Tail;
	void init() {Head = Tail =  nullptr;}
	bool isEmpty() {return Head == nullptr;}
	void createQueue(int size, FILE* file, const char* fileName) {;
		void* value = nullptr;
		while (size--) {
			value = readfile(file, fileName);
			push(value);
		}
	}
	void push(void* value) {
		if (value == nullptr) {showNotification("Du lieu khong hop le\n");}
		Node *newNode = new Node(value);
		isEmpty() ? Head = newNode : Tail->next = newNode;
		Tail = newNode;
	}
	void* pop() {
		if (isEmpty()) {return nullptr;}
		Node* temp = Head;
		void* value = temp->data;
		Head = Head->next;
		delete temp;
		temp = nullptr;
		return value;
	}
	void* deleteAfter(Node* current) {
		if (current == nullptr) {
			showNotification("\nNode khong ton tai do kich thuoc danh sach khong phu hop!");
			exit(0);
		};
		if (current->next == Tail) Tail = current;
		void* value = current->next->data;
		Node* next_next = current->next->next;
		delete current->next;
		current->next = next_next;
		return value;
	}
	void printQueue(const char* type) {
		if (Head == nullptr) 
			showNotification("\nHang doi rong");
		Node* current = Head;
		while (current != nullptr) {
			if (!strcmp(type, "don thuoc"))    {printPrescription ((Don_thuoc*)    current->data);}
			if (!strcmp(type, "hoa don nhap")) {printImportingBill((Hoa_Don_Nhap*) current->data);}
			if (!strcmp(type, "hoa don xuat")) {printExportingBill((Hoa_Don_Xuat*) current->data);}
			current =  current->next;
		}
	}
	Node* getHead() {return Head;}
};
// Menu function //
void Menu(int start, int end);
void getChoice(int &choosen, int minCase, int maxCase);
void KHOI_TAO_DS_DON_THUOC(Queue& Prescription_Queue, FILE* Don_thuoc);
void LAP_HOA_DON(Queue& IMP_BiLL_Queue, Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Nhap, FILE* Hoa_Don_Xuat);
void LAP_HOA_DON_NHAP(Queue& IMP_BiLL_Queue, FILE* Hoa_Don_Nhap);
void LAP_HOA_DON_XUAT(Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Xuat);
void TIM_KIEM_DON_THUOC(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void TIM_DON_THUOC_THEO_HOA_DON(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void IN_DANH_SACH_HOA_DON(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void TIM_THONG_TIN_DON_THUOC(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void THONG_TIN_DON_THUOC_BAN_NHIEU_NHAT_IT_NHAT(Queue EXP_BiLL_Queue, const char* type);
void* getMaxOrMin(const char* type, Queue EXP_BiLL_Queue);
void THONG_TIN_DON_THUOC_CO_THOI_GIAN_SD_DUOI_6_THANG(Queue IMP_BiLL_Queue) ;
int daysInMonth(int month, int year);
bool isValidDate(struct Date date);
bool checkExpiry(Date end);
Date getCurrentDate();
int checkMonth(Date end, int numbers);
void IN_RA_THONG_TIN_CAC_DON_THUOC_TON_SLG_LON_HON_N(int number, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void THONG_KE_THUOC_CO_THOI_HAN_SU_DUNG_DUOI_30_NGAY(Queue IMP_BiLL_Queue);
void XOA_DON_THUOC(Queue Prescription_Queue, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
#endif index_h