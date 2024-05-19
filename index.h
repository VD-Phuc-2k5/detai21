#ifndef index_h
#define index_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FILE_DON_THUOC   "thuoc.txt"
#define FILE_HOA_DON_NHAP "hoadonnhap.txt"
#define FILE_HOA_DON_XUAT "hoadonxuat.txt"
#define PRESCRIPTION_PROPERTIES 5
#define BILL_PROPERTIES 5
#define MAX_SIZE_STRING 255
#define MENU_WIDTH 154
#define SIZE 10

#pragma warning (disable : 4996)

enum Prescription_properties {maThuoc, tenThuoc, thanhPhanChinh, ghiChuCuaDonThuoc, congDung};
enum Bill_Properties {maHoaDon, ghiChuCuaHoaDon, maDonThuoc, soLo, soLuong};
enum DateType {Ngay_sx, Ngay_het_han};
enum PropertiesType {donThuoc, hoaDon};

struct Date {
	int day, month, year;
	Date() : day(0), month(0), year(0) {} 
	Date(int d, int m, int y) : day(d), month(m), year(y) {} 
};
struct Don_thuoc {char *info[PRESCRIPTION_PROPERTIES];};
struct Hoa_Don {
	Date date;
	char* info[BILL_PROPERTIES];
};
struct Hoa_Don_Xuat {
	Hoa_Don info;
	char nguoi_mua[MAX_SIZE_STRING];
};
struct Hoa_Don_Nhap {
	Hoa_Don	info;
	Date dates[2];
	char Congty_sx[MAX_SIZE_STRING];
};
struct Node {
	void* data;
	Node* next;
	Node(void* newData) : data(newData), next(nullptr) {}
};
struct Queue {
	Node* Head, * Tail;
	void init();
	bool isEmpty();
	void push(void* value);
	void* pop();
	void create(int size, FILE* file, const char* fileName);
	void print(const char* type);
	Node* getHead();
};
struct List: public Queue {
	void* deleteAfter(Node* current);
}; 

// NHOM HAM THONG BAO  :
void showNotification(const char* notification);

// NHOM HAM DOC FILE   :
void* readfile(FILE* file, const char* fileName);

// NHOM HAM LAY DU LIEU:
/*======================================================================================*/
/* + DON THUOC - HOA DON NHAP - HOA DON XUAT*/
void getProperties(FILE* file, void* variable, const char* format, int vType, int pType);
void* getPrescription(FILE* file);
Hoa_Don getBill(FILE* file);
void* getImportingBill(FILE* file);
void* getExportingBill(FILE* file);
/*======================================================================================*/
/* + NGAY THANG NAM */
void getDate(FILE* file, const char* format, Date& variable);
Date getCurrentDate();
/*======================================================================================*/
/* + MENU */
void* getMaxOrMin(const char* type, Queue EXP_BiLL_Queue);
void getChoice(int &choosen, int minCase, int maxCase);
/*======================================================================================*/
// NHOM HAM XUAT DU LIEU:
void printPrescription(Don_thuoc* value);
void printBill(Hoa_Don value, const char* string);
void printExportingBill(Hoa_Don_Xuat* value);
void printImportingBill(Hoa_Don_Nhap* value);
/*======================================================================================*/
// NHOM HAM KHAC       :
bool isValidDate(struct Date date);
bool checkExpiry(Date end);
int checkMonth(Date end, int numbers, int type);
int daysInMonth(int month, int year);
/*======================================================================================*/
// CHUC NANG CHINH CUA CHUONG TRINH
void Menu(int start, int end);
void KHOI_TAO_DS_DON_THUOC(List& Prescription_List, FILE* Don_thuoc);
void LAP_HOA_DON(Queue& IMP_BiLL_Queue, Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Nhap, FILE* Hoa_Don_Xuat);
void LAP_HOA_DON_NHAP(Queue& IMP_BiLL_Queue, FILE* Hoa_Don_Nhap);
void LAP_HOA_DON_XUAT(Queue& EXP_BiLL_Queue, FILE* Hoa_Don_Xuat);
void TIM_KIEM_DON_THUOC(List Prescription_List, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void TIM_DON_THUOC_THEO_HOA_DON(List Prescription_List, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void IN_DANH_SACH_HOA_DON(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void TIM_THONG_TIN_DON_THUOC(Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void THONG_TIN_DON_THUOC_BAN_NHIEU_NHAT_IT_NHAT(Queue EXP_BiLL_Queue, const char* type);
void THONG_TIN_DON_THUOC_CO_THOI_GIAN_SD_DUOI_6_THANG(Queue IMP_BiLL_Queue) ;
void IN_RA_THONG_TIN_CAC_DON_THUOC_TON_SLG_LON_HON_N(int number, Queue IMP_BiLL_Queue, Queue EXP_BiLL_Queue);
void THONG_KE_THUOC_CO_THOI_HAN_SU_DUNG_DUOI_30_NGAY(Queue IMP_BiLL_Queue);
void XOA_DON_THUOC(List& Prescription_List, Queue& IMP_BiLL_Queue);

#endif index_h
