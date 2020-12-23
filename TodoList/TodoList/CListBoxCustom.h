#pragma once
class CListBoxCustom : CObject
{
protected:
	//static int count;
	int index;
public: 
	CListBoxCustom() {
		index = 0;
	
	}
	int RowsSquared(int rows) {
		return rows * rows;
	}
	
};

