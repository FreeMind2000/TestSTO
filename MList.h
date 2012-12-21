// MList.h: interface for the MList class.
//------------------------------------------
// �������� ������ ������������ ������
//------------------------------------------
// ����������: �������� �.�.
// ����: 03.07.2006�
// ��������� ���������: 28.12.2007�
// 13.05.2012 - ������� GetPtrCurObj()
// 14.05.2012 - RemoveCurObj()
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef MLIST_H
#define MLIST_H


//-----------------------------------------------------------------------------------
//
template <class T>
class MListObj  
{
public:
	T* obj;

	MListObj *next;
};



//-----------------------------------------------------------------------------------
//
template <class T>
class MList  
{
public:
	MListObj<T> *CurObj;		// ��������� �� ������� ������ � ������
	MListObj<T> *PrevObj;		// ��������� �� ���������� ������ � ������
	
	MListObj<T> *Head;			// ������ ������ - ��������� �� ���������
								// ����������� � ������ ������

	unsigned int Count;			// ���-�� �������� � ������


public:
	MList();
	~MList();
	void Create();
	void Destroy();
	void Add(T *new_obj);
	void DeleteCurObj();        // �������� �� ������� CurObj �� next
	void RemoveCurObj();        // ������� ������� ������(�������) �� ������ ��� �� ��� � DeleteCurObj()
								// �� ��� ���� ��� ������(��������� �� ������) �� delet'��!!!
	void MoveFirst();
	void MoveNext();
	bool Eof();
	T& GetCurObj();
	T* GetPtrCurObj();
    unsigned int CicleCount();  // ���������� Count � MoveFirst ��� ������
};


//void _mymain();

//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// MList
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------
//
template <class T>
MList<T>::MList()
{
	Create();
}

//-----------------------------------------------------------------------------------
//
template <class T>
MList<T>::~MList()
{
	Destroy();
}



//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::Create()
{
	CurObj  = NULL;
	PrevObj = NULL;
	Head	= NULL;
	Count	= 0;
}


//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::Add(T *new_obj)
{
	// ������� ������ ������� ��� new_obj
	MListObj<T> *p = new (MListObj<T>);

	// ������������ 
	p->obj = new_obj;

	// ������������ � ������

	PrevObj = Head;

	p->next = Head;
	Head	= p;

	CurObj = Head;

	Count++;
}


//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::DeleteCurObj()
{

	if (CurObj == NULL) return;

	if (PrevObj != NULL)
	{
		PrevObj->next = CurObj->next;
		delete CurObj->obj;
		delete CurObj;

		CurObj = PrevObj->next;
	}
	else
	{
		Head = CurObj->next;
		delete CurObj->obj;
		delete CurObj;

		CurObj = Head;
	}


//	static int x=1;
//	AnsiString s; s.printf("delete - %d", x);
//	OutputDebugString(s.c_str());
//	x++;

	Count--;
}


//-----------------------------------------------------------------------------------
// ������� ������� ������ �� ������ ��� �� ��� � DeleteCurObj()
// �� ��� ���� ��� ������(��������� �� ������) �� delet'��!!!
template <class T>
void MList<T>::RemoveCurObj()
{

	if (CurObj == NULL) return;

	if (PrevObj != NULL)
	{
		PrevObj->next = CurObj->next;
		//delete CurObj->obj;
		delete CurObj;

		CurObj = PrevObj->next;
	}
	else
	{
		Head = CurObj->next;
		//delete CurObj->obj;
		delete CurObj;

		CurObj = Head;
	}


//	static int x=1;
//	AnsiString s; s.printf("delete - %d", x);
//	OutputDebugString(s.c_str());
//	x++;

	Count--;
}


//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::MoveFirst()
{
	CurObj = Head;
	PrevObj = NULL;
}




//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::MoveNext()
{
	PrevObj = CurObj;
	CurObj	= CurObj->next;
}

//-----------------------------------------------------------------------------------
//
template <class T>
bool MList<T>::Eof()
{
	if (CurObj == NULL) return(TRUE);
	
	return (FALSE);
}


//-----------------------------------------------------------------------------------
//
template <class T>
void MList<T>::Destroy()
{
	MoveFirst();

	while(!Eof())
	{
		DeleteCurObj();
	}
}


//-----------------------------------------------------------------------------------
//
template <class T>
T& MList<T>::GetCurObj()
{
	T &r = *CurObj->obj;

	return (r);
}

//-----------------------------------------------------------------------------------
//
template <class T>
T* MList<T>::GetPtrCurObj()
{
	return (CurObj->obj);
}

//----------------------------------------------------------------------------
// ���������� Count � MoveFirst ��� ������
template <class T>
unsigned int MList<T>::CicleCount()
{
    MoveFirst();
    return (Count);
}
//-----------------------------------------------------------------------------------

#endif


/////////////////////////////////////////////////////////////////////////////////////
//
//	�������������
//
/////////////////////////////////////////////////////////////////////////////////////

/*

class MyClass
{
public:
	int x,y;

	static MyClass* Create()
	{
		MyClass *p = new (MyClass);
		return p;
	}

	void Init(int xx, int yy) {x=xx; y=yy;}

	void Show()
	{
		char buf[200];

		sprintf(buf, "x=%d; y=%d;\n", x, y);

		::AfxMessageBox(buf);
	}

	void operator = (MyClass& v1)
	{
		this->x = v1.x;
		this->y = v1.y;
	}
};



void _mymain()
{
	unsigned int i;
	MList<MyClass> MyList;
//	MyList.Create();

	// �������� - ����������
	for (i=0; i<5; i++)
	{
		MyClass *ptr = MyClass::Create(); // new (MyClass); //

		ptr->Init(i, i);				  // ptr->x = i; ptr->y = i; //

		MyList.Add(ptr);
	}

	// �������������
	//MyList.MoveFirst();
	//for (i=0; i<MyList.Count; i++)
    // ��� ������
	for (i=MyList.CicleCount(); i>0; i--)
	{
		MyList.GetCurObj().Show();		  // MyList.CurObj->obj.Show(); //

		MyList.MoveNext();
	}

	// ��������
	MyList.MoveFirst();
	for (i=0; i<MyList.Count; i++)
	{
		if (MyList.GetCurObj().x == 3)	  // if (MyList->Obj.x == 3)
		{
			MyList.DeleteCurObj();
			break;
		}

		MyList.MoveNext();
	}

	// �������������
	MyList.MoveFirst();
//	for (i=0; i<MyList.Count; i++)
	while(!MyList.Eof())
	{
		MyList.GetCurObj().Show();		  // MyList.CurObj->obj.Show(); //

		MyList.MoveNext();
	}


	// �����������
	MyList.Destroy();

}
*/


