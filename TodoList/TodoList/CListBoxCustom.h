#pragma once
class CListBoxCustom : CObject
{
private:
	static int count;

public:
	//DECLARE_SERIAL(CListBoxCustom)
	static int clickCount() {
		return count++;
	}
	static int getCount() {
		return count;
	}

	int RowsSquared(int rows) {
		return rows * rows;
	}

	//void Serialize(CArchive& archive);
	
};


