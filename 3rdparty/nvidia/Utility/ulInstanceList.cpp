/******************************************************************************
ulInstanceList.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:



******************************************************************************/
#include <ul.h>
/*

template <class _BaseClass, class _TypePtr> 
class InstanceList
{
  friend class Instance;

public:

  ~InstanceList() { EraseList(); }

  class Instance
  {
    friend class InstanceList<_BaseClass, _TypePtr>;
  public:
    Instance * GetNext() {return m_next;}
    Instance * GetPrev() {return m_prev;}
    void GetNext(Instance * n) {m_next = n;}
    void GetPrev(Instance * n) {m_prev = n;}

    Instance(Instance *p, Instance *n, _TypePtr v)
    {
      set(p,n,v);
    }

    void set(Instance *p, Instance *n, _TypePtr v)
    {
      m_prev = p;
      m_next = n;
      data = v;
    }    

    operator _TypePtr() const
    {
      return data;
    }
    
    _TypePtr operator->() const
    {
      ATLASSERT(data!=NULL);
      return data;
    }

    bool operator==(_TypePtr pT) const
    {
      return data == pT;
    }
    
  protected:

    Instance * m_next;   
    Instance * m_prev;   

  private:
    _TypePtr data;
  };


  typedef Instance * iterator;
  
  Instance * m_head;
  Instance * m_tail;
  _TypePtr m_current;
  int m_currentnum;
  int m_NumNodes;
  
  
public:

  InstanceList();
  Instance * Head() { return m_head;}
  Instance * Tail() { return m_tail;}
  bool IsEmpty() { return (Size() == 0);}
  bool empty() { return (Size() == 0);}
  int Size() {return m_NumNodes;}
  int size() {return m_NumNodes;}

  _TypePtr SetCurrentByNumber(const int & n);
  void SetCurrent(_TypePtr node);
  void SetCurrentAndNumber(_TypePtr node);
  _TypePtr GetCurrent();
  int GetCurrentNumber();
  _TypePtr GetEntryByNumber(int n);
  int FindNode(_TypePtr f);
  iterator Erase(iterator p);
  void EraseList();

  void DeleteData(_BaseClass * b);
  void DeleteData(ulComPtr<_BaseClass> b);


  void DeleteListContents();
  void DeleteAndEraseList();
  void SoilList();
  void RefreshList();
  void BuildList();
  void DestroyList();
  void ApplyFunc(ulFuncPtrv function);
  void ApplyFunc(ulFuncPtrb function, bool b);

  // insert at the end of the list
  iterator Insert(_TypePtr data);

};




////  Use Com for references
template <class _BaseClass, class _TypePtr> 
InstanceList <_BaseClass,_TypePtr>::InstanceList() 
{
  m_head = 0; 
  m_tail = 0; 
  m_NumNodes  = 0; 
  m_current = 0; 
  m_currentnum = -1;
}


template <class _BaseClass,class _TypePtr>
_TypePtr 
InstanceList <_BaseClass,_TypePtr>::SetCurrentByNumber(const int & n) 
{
  m_current = GetEntryByNumber(n);
  m_currentnum = n;
  return m_current;
}

template <class _BaseClass, class _TypePtr>
void  
InstanceList <_BaseClass, _TypePtr>::SetCurrent(_TypePtr node) 
{
  m_current = node;
}


template <class _BaseClass, class _TypePtr> 
void 
InstanceList <_BaseClass, _TypePtr>::SetCurrentAndNumber(_TypePtr node) 
{
  m_currentnum = FindNode(node);
  m_current = node;
}
template <class _BaseClass, class _TypePtr> 
_TypePtr
InstanceList <_BaseClass, _TypePtr>::   GetCurrent() 
{
  return m_current;
}


template <class _BaseClass, class _TypePtr> 
int 
InstanceList <_BaseClass, _TypePtr>::GetCurrentNumber() 
{
  return m_currentnum;
}


template <class _BaseClass, class _TypePtr> 
_TypePtr
InstanceList <_BaseClass, _TypePtr>::GetEntryByNumber(int n)
{
  iterator p;
  int i = 0;
  
  for(p = Head(); p;  NextItem(p))
  {
    if (i == n)
    {
      if (p)
        return p->data;
      else
        return 0;
    }
    i++;
  }
  return 0;
}

template <class _BaseClass, class _TypePtr> 
int
InstanceList <_BaseClass, _TypePtr>::FindNode(_TypePtr f)
{
  iterator p;
  int i = 0;
  
  for(p = Head(); p;  NextItem(p))
  {
    if (f == p->data)
      return i;
    i++;
  }
  return -1; // not found
}





template <class _BaseClass, class _TypePtr> 
void 
InstanceList <_BaseClass, _TypePtr>::  EraseList() 
{
  iterator p = m_head;
  while (p)
    p = Erase(p);
  if (m_head != 0)
    m_head = 0;
  
}










template <class _BaseClass, class _TypePtr> 
void
InstanceList <_BaseClass, _TypePtr>::SoilList() 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->SetChanged(true);
  }
}

template <class _BaseClass, class _TypePtr> 
void
InstanceList <_BaseClass, _TypePtr>::RefreshList() 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->Refresh();
  }
}

template <class _BaseClass, class _TypePtr> 
void
InstanceList <_BaseClass, _TypePtr>::BuildList() 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->Build();
  }
}
template <class _BaseClass, class _TypePtr>
void 
InstanceList <_BaseClass, _TypePtr>:: DestroyList() 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->Destroy();
  }
}

                
template <class _BaseClass, class _TypePtr>
void 
InstanceList <_BaseClass, _TypePtr>::ApplyFunc(ulFuncPtrv function) 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->function();
  }
}

template <class _BaseClass, class _TypePtr> 
void
InstanceList <_BaseClass, _TypePtr>::ApplyFunc(ulFuncPtrb function, bool b) 
{
  iterator p;
  
  for(p = Head(); p;  NextItem(p))
  {
    (*p)->function(b);
  }
}


// insert at the end of the list
template <class _BaseClass, class _TypePtr>
InstanceList <_BaseClass, _TypePtr>::iterator 
InstanceList <_BaseClass, _TypePtr>::Insert(_TypePtr data)
{
  if (data == 0)
    return 0;
  Instance * p = new Instance(0,0,data);
  
  if (p == 0)
    return 0;
  if (m_head)  
  { 
    m_tail->m_next = p;
    p->m_prev = m_tail;
    
    // new tail
    m_tail = p;
    m_tail->m_next = 0;
  } 
  else // no head so insert
  { 
    m_tail = m_head = p; 
    m_head->m_next = 0;
    m_head->m_prev = 0; 
  }
  m_NumNodes++;    
  
  if (m_tail && m_tail->m_next != 0)
    m_tail->m_next = 0;
  
  if (m_head && m_head->m_prev != 0)
    m_head->m_prev = 0; 
  
  return p;
}







template <class _BaseClass, class _TypePtr>
InstanceList <_BaseClass, _TypePtr>::iterator 
InstanceList <_BaseClass, _TypePtr>::Erase(iterator p) 
{
  iterator nextnode = 0;  // return next in list
  if (p)
  {
    nextnode = p->GetNext();  // return next in list
    
    if (p == m_head)
    {
      // at start
      m_head = m_head->m_next;  // inc head
      if (m_head)
        m_head->m_prev = 0;     // terminate start
      else
        m_tail = 0;   // no more list
      
    }
    else if (p == m_tail)
    {
      m_tail = m_tail->m_prev;
      if (m_tail)
        m_tail->m_next = 0;     // terminate end
      else
        m_head = 0;
      
    }
    else
    {
      // at start
      Instance * prevnode = p->m_prev;  // return prev in list
      
      prevnode->m_next = nextnode;  
      nextnode->m_prev = prevnode;  
    }
    if (*p == m_current)
    {
      m_current = 0;
      m_currentnum = -1;
    }
    
    delete p;
    p = 0;
    m_NumNodes--;
  }
  
  if (m_tail && m_tail->m_next != 0)
    m_tail->m_next = 0;
  
  if (m_head && m_head->m_prev != 0)
    m_head->m_prev = 0; 
  
  return nextnode;
}
template <class _BaseClass, class _TypePtr> 
void
InstanceList <_BaseClass, _TypePtr>::DeleteAndEraseList() 
{
  DeleteListContents();
  EraseList();
}



template <class _BaseClass, class _TypePtr> 
void 
InstanceList <_BaseClass, _TypePtr>::DeleteListContents() 
{
  iterator p;
  _TypePtr data;
  
  for(p = m_head; p; NextItem(p))
  {
    DeleteData(*p);
  }
}


template <class _BaseClass,class _TypePtr>
void 
InstanceList <_BaseClass, _TypePtr>::DeleteData(_BaseClass * p)
{
  if (p)
    delete p;
}

template <class _BaseClass, class _TypePtr>
void 
InstanceList <_BaseClass, _TypePtr>::DeleteData(ulComPtr<_BaseClass> p)
{
}


*/