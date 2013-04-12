#ifndef GLARE_PTR_H
#define GLARE_PTR_H

#include "CRTDBG.H"


struct PtrRefCount
{
	private:
		int count;

	public:
		PtrRefCount()
			:count(0)
		{}

		void Add()
		{ this->count++; }
		int Release()
		{ return --this->count;}
		void Reset()
		{ this->count = 0; }
};



#pragma region PTR


	//! Smart pointer for a regular object.
	/** 
	*	Regular objects, objects that is deleted normaly (ie not COM objects, och array pointers) 
	*	can use this class to easy the use of dynamic memory 
	*/
	template<typename T>
	class SmartPtrStd
	{
		private:
			PtrRefCount	*_rc;
			T					*_ptr;

		public:
			SmartPtrStd						();
			SmartPtrStd						(T* p);
			SmartPtrStd						(const SmartPtrStd&);
			virtual~SmartPtrStd				();
			SmartPtrStd&	operator=		(const SmartPtrStd<T>&);
			SmartPtrStd&	operator=		(T*);
			bool			operator==		(const SmartPtrStd<T>&);
			bool			operator==		(const T&);
			T&				operator*		();
			T*				operator->		();
							operator T*		();


			/** Adds a new pointer.
				If the pointer is occupied, this will not do anything and return false. (See Swap instead) */
			bool Add(T*);

			/** Changes the current pointer with a new one.
				The old pointer will be deleted, thus making the content useless in other places. */
			void Swap(T*);

			/** Checks if the pointer is valid (not NULL)
				Returns true for valid, else false. */
			bool IsValid();

			/** Use with cation! 
				Destroys the pointer and returns the memory allocated. */
			void Destroy		();
	};

#pragma region DECLARATION


		template<typename T>
	SmartPtrStd<T>::SmartPtrStd()
		:_rc(0), _ptr(0)
	{ }
		template<typename T>
	SmartPtrStd<T>::SmartPtrStd(T* p)
		:_ptr(p)
	{ 
		this->_rc = new PtrRefCount();
		this->_rc->Add();
	}
		template<typename T>
	SmartPtrStd<T>::SmartPtrStd(const SmartPtrStd& d)
		:_ptr(d._ptr), _rc(d._rc)
	{
		if(this->_rc)
			this->_rc->Add();
	}
		template<typename T>
	SmartPtrStd<T>::~SmartPtrStd()
	{
		if (this->_rc && this->_rc->Release() == 0)
		{
			Destroy();
		}
	}


#pragma region OPERATOR


		template<typename T>
	SmartPtrStd<T>& SmartPtrStd<T>::operator= (const SmartPtrStd<T>& p)
	{
		if (this != &p)
		{
			//Last to go?
			if(this->_rc && this->_rc->Release() == 0)
			{
				//Call child specific
				Destroy();
			}
		

			this->_ptr = p._ptr;
			this->_rc = p._rc;
			this->_rc->Add();
		}
		return *this;
	}
		template<typename T>
	SmartPtrStd<T>& SmartPtrStd<T>::operator= (T* p)
	{
		if (this->_ptr != p)
		{
			//Last to go?
			if(this->_rc)
			{
				if(this->_rc->Release() == 0)
				{
					//Call child specific
					Destroy();
				}
			}
			else
			{
				this->_rc = new PtrRefCount();
			}
		

			this->_ptr = p;
			this->_rc->Add();
		}
		return *this;
	}
		template<typename T>
	bool SmartPtrStd<T>::operator== (const SmartPtrStd<T>& d)
	{
		return d._ptr == this->_ptr;
	}
		template<typename T>
	bool SmartPtrStd<T>::operator== (const T& p)
	{
		return &p == this->_ptr;
	}
		template<typename T>
	T& SmartPtrStd<T>::operator* ()
	{
		return *this->_ptr;
	}
		template<typename T>
	T* SmartPtrStd<T>::operator-> ()
	{
		return this->_ptr;
	}
		template<typename T>
	SmartPtrStd<T>::operator T* ()
	{
		return this->_ptr;
	}

#pragma endregion

	template<typename T>
bool SmartPtrStd<T>::Add(T* p)
{
	if(this->_ptr)
		return false;

	if(!this->_rc)
		this->_rc = new PtrRefCount();

	this->_ptr = p;
	this->_rc->Add();

	return true;
}
	template<typename T>
void SmartPtrStd<T>::Swap(T* p)
{
	if (this->_ptr != p)
	{
		if(this->_ptr)
			delete this->_ptr();
		if(!this->_rc)
			this->_rc = new PtrRefCount();
		this->_rc->Reset();

		this->_ptr = p;
		this->_rc->Add();
	}
}
	template<typename T>
bool SmartPtrStd<T>::IsValid()
{
	return (this->_ptr != NULL) ?	true : false;
}

		template<typename T>
	void SmartPtrStd<T>::Destroy()
	{
		delete this->_rc;
		this->_rc = NULL;
		delete this->_ptr;
		this->_ptr = NULL;
	}


#pragma endregion

#pragma endregion



#pragma region PTR_COM

	//! Smart pointer for COM object.
	/** 
	*	Regular objects, objects that is released (->Release()) 
	*	can use this class to easy the use of dynamic memory 
	*/
	template<typename T>
	class SmartPtrCom
	{
		private:
			PtrRefCount						*_rc;
			T								*_ptr;

		public:
			SmartPtrCom						();
			SmartPtrCom						(T* p);
			SmartPtrCom						(const SmartPtrCom&);
			virtual~SmartPtrCom				();

			SmartPtrCom&	operator=		(const SmartPtrCom<T>&);
			bool			operator==		(const SmartPtrCom<T>&);
			bool			operator==		(const T&);
			T&				operator*		();
			T*				operator->		();
							operator T**	();
							operator T*		();

			/** Adds a new pointer.
				If the pointer is occupied, this will not do anything and return false. (See Swap instead) */
			bool Add(T*);

			/** Changes the current pointer with a new one.
				The old pointer will be deleted, thus making the content useless in other places. */
			void Swap(T*);

			/** Checks if the pointer is valid (not NULL)
				Returns true for valid, else false. */
			bool IsValid();

			/** Use with cation! 
				Destroys the pointer and returns the memory allocated. */
			void Destroy					();
	};


#pragma region DECLARATION

		template<typename T>
	SmartPtrCom<T>::SmartPtrCom()
		:_rc(0), _ptr(0)
	{ }
		template<typename T>
	SmartPtrCom<T>::SmartPtrCom(T* p)
		:_ptr(p)
	{ 
		this->_rc = new PtrRefCount();
		this->_rc->Add();
	}
		template<typename T>
	SmartPtrCom<T>::SmartPtrCom(const SmartPtrCom& d)
		:_ptr(d._ptr), _rc(d._rc)
	{
		if(this->_rc)
			this->_rc->Add();
	}
		template<typename T>
	SmartPtrCom<T>::~SmartPtrCom()
	{
		if (this->_rc && this->_rc->Release() == 0)
		{
			Destroy();
		}
	}


	#pragma region OVERLOADING

			template<typename T>
		SmartPtrCom<T>& SmartPtrCom<T>::operator= (const SmartPtrCom<T>& p)
		{
			if (this != &p)
			{
				//Last to go?
				if(this->_rc && this->_rc->Release() == 0)
				{
					//Call child specific
					Destroy();
				}
		
				this->_ptr = p._ptr;
				this->_rc = p._rc;
				this->_rc->Add();
			}
			return *this;
		}
			template<typename T>
		bool SmartPtrCom<T>::operator== (const SmartPtrCom<T>& d)
		{
			return d._ptr == this->_ptr;
		}
			template<typename T>
		bool SmartPtrCom<T>::operator== (const T& p)
		{
			return &p == this->_ptr;
		}
			template<typename T>
		T& SmartPtrCom<T>::operator* ()
		{
			return *this->_ptr;
		}
			template<typename T>
		T* SmartPtrCom<T>::operator-> ()
		{
			return this->_ptr;
		}
			template<typename T>
		SmartPtrCom<T>::operator T** ()
		{
			return &this->_ptr;
		}
			template<typename T>
		SmartPtrCom<T>::operator T* ()
		{
			return this->_ptr;
		}

	#pragma endregion


	template<typename T>
bool SmartPtrCom<T>::Add(T* p)
{
	if(this->_ptr)
		return false;

	if(!this->_rc)
		this->_rc = new PtrRefCount();

	this->_ptr = p;
	this->_rc->Add();

	return true;
}
	template<typename T>
void SmartPtrCom<T>::Swap(T* p)
{
	if (this->_ptr != p)
	{
		if(this->_ptr)
			delete this->_ptr();
		if(!this->_rc)
			this->_rc = new PtrRefCount();
		this->_rc->Reset();

		this->_ptr = p;
		this->_rc->Add();
	}
}
	template<typename T>
bool SmartPtrCom<T>::IsValid()
{
	return (this->_ptr != NULL) ?	true : false;
}


		template<typename T>
	void SmartPtrCom<T>::Destroy()
	{
		delete this->_rc;
		this->_rc = NULL;
		if(this->_ptr)
		{
			this->_ptr->Release();
			this->_ptr = NULL;
		}
	}

#pragma endregion


#pragma endregion



#pragma region PTR_ARRAY


	//! Smart pointer for a regular object.
	/** 
	*	Regular objects, objects that is deleted normaly (ie not COM objects, och array pointers) 
	*	can use this class to easy the use of dynamic memory 
	*/
	template<typename T>
	class SmartPtrArr
	{
		private:
			PtrRefCount							*_rc;
			T									*_ptr;

		public:
			SmartPtrArr							();
			SmartPtrArr							(const SmartPtrArr&);
			virtual~SmartPtrArr					();

			SmartPtrArr&	operator=			(const SmartPtrArr<T>&);
			SmartPtrArr&	operator=			(T*);
			bool			operator==			(const SmartPtrArr<T>&);
			bool			operator==			(const T*);
			T&				operator[]			(int i);
							operator T*			();


			/** Adds a new pointer.
				If the pointer is occupied, this will not do anything and return false. (See Swap instead) */
			bool Add(T*);

			/** Changes the current pointer with a new one.
				The old pointer will be deleted, thus making the content useless in other places. */
			void Swap(T*);

			/** Checks if the pointer is valid (not NULL)
				Returns true for valid, else false. */
			bool IsValid();

			/** Use with cation! 
				Destroys the pointer and returns the memory allocated. */
			void Destroy		();
	};

#pragma region DECLARATION


		template<typename T>
	SmartPtrArr<T>::SmartPtrArr()
		:_rc(0), _ptr(0)
	{ this->_rc = new PtrRefCount(); }
		template<typename T>
	SmartPtrArr<T>::SmartPtrArr(const SmartPtrArr& d)
		:_ptr(d._ptr)
	{
		if(this->_rc)
			delete this->_rc;
			
		this->_rc = d._rc;
		this->_rc->Add();
	}
		template<typename T>
	SmartPtrArr<T>::~SmartPtrArr()
	{
		if (this->_rc && this->_rc->Release() == 0)
		{
			Destroy();
		}
	}


#pragma region OPERATOR


		template<typename T>
	SmartPtrArr<T>& SmartPtrArr<T>::operator= (const SmartPtrArr<T>& p)
	{
		if (this != &p)
		{
			if(this->_rc && this->_rc->Release() == 0)
			{
				Destroy();
			}
		
			this->_ptr = p._ptr;
			this->_rc = p._rc;
			this->_rc->Add();
		}
		return *this;
	}
	template<typename T>
	SmartPtrArr<T>&	SmartPtrArr<T>::operator=(T* ptr)
	{

		delete [] this->_ptr;
		this->_ptr = ptr;

		if(!this->_rc)
			this->_rc = new PtrRefCount();
		
		this->_rc->Reset();
		this->_rc->Add();

		return *this;
	}
		template<typename T>
	bool SmartPtrArr<T>::operator== (const SmartPtrArr<T>& d)
	{
		return d._ptr == this->_ptr;
	}
		template<typename T>
	bool SmartPtrArr<T>::operator== (const T* p)
	{
		return p == this->_ptr;
	}
		template<typename T>
	T& SmartPtrArr<T>::operator[] (int i)
	{
		return this->_ptr[i];
	}
	
		template<typename T>
	SmartPtrArr<T>::operator T*()
	{
		return &this->_ptr[0];
	}

#pragma endregion

		template<typename T>
	bool SmartPtrArr<T>::Add(T* p)
	{
		if(this->_ptr)
			return false;

		if(!this->_rc)
			this->_rc = new PtrRefCount();

		this->_ptr = p;
		this->_rc->Add();

		return true;
	}
		template<typename T>
	void SmartPtrArr<T>::Swap(T* p)
	{
		if (this->_ptr != p)
		{
			if(this->_ptr)
				delete [] this->_ptr();
			if(!this->_rc)
				this->_rc = new PtrRefCount();
			this->_rc->Reset();

			this->_ptr = p;
			this->_rc->Add();
		}
	}
		template<typename T>
	bool SmartPtrArr<T>::IsValid()
	{
		return (this->_ptr != NULL)  ?	true : false;
	}

		template<typename T>
	void SmartPtrArr<T>::Destroy()
	{
		delete this->_rc;
		this->_rc = NULL;
		delete [] this->_ptr;
		this->_ptr = NULL;
	}


#pragma endregion

#pragma endregion




#endif
   