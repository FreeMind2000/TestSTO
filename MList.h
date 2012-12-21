// MList.h: interface for the MList class.
//------------------------------------------
// Описание класса односвязного списка
//------------------------------------------
// Разработал: Меркелов А.А.
// Дата: 03.07.2006г
// Последнее изменение: 28.12.2007г
// 13.05.2012 - добавил GetPtrCurObj()
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
	MListObj<T> *CurObj;		// Указатель на текущий объект в списке
	MListObj<T> *PrevObj;		// Указатель на предыдущий объект в списке
	
	MListObj<T> *Head;			// Голова списка - указатель на последний
								// добавленный в список объект

	unsigned int Count;			// Кол-во объектов в списке


public:
	MList();
	~MList();
	void Create();
	void Destroy();
	void Add(T *new_obj);
	void DeleteCurObj();        // удаление со сдвигом CurObj на next
	void RemoveCurObj();        // удаляем текущий объект(обертку) из списка так же как в DeleteCurObj()
								// но при этом сам объект(указатель на объект) не delet'им!!!
	void MoveFirst();
	void MoveNext();
	bool Eof();
	T& GetCurObj();
	T* GetPtrCurObj();
    unsigned int CicleCount();  // возвращаем Count и MoveFirst для циклов
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
	// создаем объект обертку для new_obj
	MListObj<T> *p = new (MListObj<T>);

	// обварачиваем 
	p->obj = new_obj;

	// присоединяем к списку

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
// удаляем текущий объект из списка так же как в DeleteCurObj()
// но при этом сам объект(указатель на объект) не delet'им!!!
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
// возвращаем Count и MoveFirst для циклов
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
//	Использование
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

	// создание - добавление
	for (i=0; i<5; i++)
	{
		MyClass *ptr = MyClass::Create(); // new (MyClass); //

		ptr->Init(i, i);				  // ptr->x = i; ptr->y = i; //

		MyList.Add(ptr);
	}

	// использование
	//MyList.MoveFirst();
	//for (i=0; i<MyList.Count; i++)
    // или короче
	for (i=MyList.CicleCount(); i>0; i--)
	{
		MyList.GetCurObj().Show();		  // MyList.CurObj->obj.Show(); //

		MyList.MoveNext();
	}

	// удаление
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

	// использование
	MyList.MoveFirst();
//	for (i=0; i<MyList.Count; i++)
	while(!MyList.Eof())
	{
		MyList.GetCurObj().Show();		  // MyList.CurObj->obj.Show(); //

		MyList.MoveNext();
	}


	// уничтожение
	MyList.Destroy();

}
*/


