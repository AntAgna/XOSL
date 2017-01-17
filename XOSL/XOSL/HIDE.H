/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#ifndef HideH
#define HideH

#include <newdefs.h>
#include <gui.h>
#include <screen.h>
#include <ptab.h>
#include <items.h>

class CHidingDialog {
public:
	CHidingDialog(CScreen &ScreenToUse, CPartList &PartListToUse, CBootItems &BootItemsToUse);
	~CHidingDialog();

	void ShowModal(CForm *Owner, int ItemIndex);
private:
	CPartList &PartList;
	CScreen &Screen;
	CBootItems &BootItems;

	bool Initialized;

	CForm *Owner;

	unsigned char LocalHideList[24][56];

	int BootItemIndex;
	void UpdateBootItemList();
	void UpdateHidingList();
	void SetCaption();

	void UpdateLocalHideList();
	void UpdateBootItemHideList();


private:	// controls
	CForm *Form;

	CButton *ApplyBtn;
	CButton *CancelBtn;
	CListBox *PartListBox;
	CScrollBar *PartScrollBar;
	CCheckBox *HideCheckBox;
	CButton *HideAllBtn;
	CButton *HideNoneBtn;
	CListBox *ItemsListBox;
	CScrollBar *ItemsScrollBar;
	CButton *CopyFromBtn;


private:

	void CreateControls();
	void InitializeControls();
	void InstallControls();

	void InitPartListBox();
	void PositionDialog();

	void AddDisk(int Row, int Drive);
	void AddSystem(int Row, const char *FSName);
	void AddSize(int Row, long Sectors);
	void AddVolume(int Row, const char *VolumeLabel);

	void CloseDialog();

	static void ApplyBtnClick(CHidingDialog &HidingDialog);
	static void CancelBtnClick(CHidingDialog &HidingDialog);
	static void SelectPartition(CHidingDialog &HidingDialog, int ItemIndex);
	static void SelectBootItem(CHidingDialog &HidingDialog, int ItemIndex);
	
	static void HideCheckBoxChange(CHidingDialog &HidingDialog, bool Checked);
	static void HideAllBtnClick(CHidingDialog &HidingDialog);
	static void HideNoneBtnClick(CHidingDialog &HidingDialog);
	static void CopyFromBtnClick(CHidingDialog &HidingDialog);


};



#endif
