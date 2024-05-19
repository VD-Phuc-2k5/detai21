#include "index.h"

int main() {
	List Prescription_Queue;
	Queue IMP_BiLL_Queue, EXP_BiLL_Queue;
	Prescription_Queue.init(); IMP_BiLL_Queue.init(), EXP_BiLL_Queue.init();
	FILE* don_thuoc    = fopen(FILE_DON_THUOC  , "r");
	FILE* hoa_don_nhap = fopen(FILE_HOA_DON_NHAP, "r");
	FILE* hoa_don_xuat = fopen(FILE_HOA_DON_XUAT, "r");
	int choice;
	do {
		system("cls");
		Menu(1, 8);
		getChoice(choice, 0, 8);
		switch (choice)	{
			case 0: exit(0); break;
			case 1: KHOI_TAO_DS_DON_THUOC(Prescription_Queue, don_thuoc); break;
			case 2: LAP_HOA_DON(IMP_BiLL_Queue, EXP_BiLL_Queue, hoa_don_nhap, hoa_don_xuat); break;
			case 3: TIM_KIEM_DON_THUOC(Prescription_Queue, IMP_BiLL_Queue, EXP_BiLL_Queue); break;
			case 4: IN_DANH_SACH_HOA_DON(IMP_BiLL_Queue, EXP_BiLL_Queue); break;
			case 5: TIM_THONG_TIN_DON_THUOC(IMP_BiLL_Queue, EXP_BiLL_Queue); break;
			case 6: IN_RA_THONG_TIN_CAC_DON_THUOC_TON_SLG_LON_HON_N(100, IMP_BiLL_Queue, EXP_BiLL_Queue); break;
			case 7: THONG_KE_THUOC_CO_THOI_HAN_SU_DUNG_DUOI_30_NGAY(IMP_BiLL_Queue); break;
			case 8: XOA_DON_THUOC(Prescription_Queue, IMP_BiLL_Queue); break;
		}
		system("pause>0");
	} while(choice);
	fclose(don_thuoc); fclose(hoa_don_nhap); fclose(hoa_don_xuat);
	return (0);
}
