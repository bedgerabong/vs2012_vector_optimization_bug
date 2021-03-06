// vs2012_vector_optimization_bug.cpp : Defines the entry point for the console application.
//
#include <stdlib.h> //size_t, malloc, free
#include <new> // bad_alloc, bad_array_new_length
#include <memory>
#include <iostream>
#include <vector>

void print_allocator_construct_call(char const * func, int line)
{
  std::cout << func << "(at line " << line << ") Whoops...this function should be elided!" << std::endl;
}

#define PRINT_CONSTRUCT_CALL(func) print_allocator_construct_call(func,__LINE__);

#if _MSC_VER == 1500 // VS2008

  // The simplest standards-conforming allocator
template<typename T>
class aligned_allocator
{	// generic aligned_allocator for type double
public:

  typedef T value_type;

  typedef value_type * pointer;
  typedef value_type const *const_pointer;

	typedef value_type& reference;
	typedef const value_type& const_reference;

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  template<class U>
  struct rebind
  {	// convert this type to allocator<_Other>
    typedef aligned_allocator<U> other;
  };

  aligned_allocator()
  {	// construct default aligned_allocator (do nothing)
  }

  aligned_allocator(aligned_allocator const &)
  {	// construct default aligned_allocator (do nothing)
  }

  template<typename U>
  aligned_allocator(aligned_allocator<U> const &)
  {	// construct default aligned_allocator (do nothing)
  }

  bool operator==(aligned_allocator const &) const { return true; }

  bool operator!=(aligned_allocator const &) const { return true; }

  pointer allocate(size_t size) const
  {	// allocate array of _Count elements
    return static_cast<T*>(::operator new(size * sizeof(T)));
  }

  void deallocate(pointer p, size_t) const
  {	// deallocate object at _Ptr, ignore size
    ::operator delete(p);
  }

  void construct(double *p)
  {	// default construct object at _Ptr
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double();
  }

  void construct(double *p, const double& val)
  {	// construct object at _Ptr with value _Val
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double(val);
  }

  template<class U>
  void destroy(U * p)
  {	// destroy object at _Ptr
    p->~U();
  }

	size_t max_size() const _THROW0()
	{	// estimate maximum array size
		return ((size_t)(-1) / sizeof(double));
	}

};

#elif _MSC_VER == 1700 // VS2012

namespace std {

  template<>
    class allocator<double>
        : public _Allocator_base<double>
    {	// generic allocator for objects of class double
    public:
        typedef allocator<double> other;

        typedef _Allocator_base<double> _Mybase;
        typedef _Mybase::value_type value_type;

        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef void *void_pointer;
        typedef const void *const_void_pointer;

        typedef value_type& reference;
        typedef const value_type& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef false_type propagate_on_container_copy_assignment;
        typedef false_type propagate_on_container_move_assignment;
        typedef false_type propagate_on_container_swap;

        allocator<double> select_on_container_copy_construction() const
        {	// return this allocator
            return (*this);
        }

        template<class _Other>
        struct rebind
        {	// convert this type to allocator<_Other>
            typedef allocator<_Other> other;
        };

        pointer address(reference _Val) const _NOEXCEPT
        {	// return address of mutable _Val
            return (_STD addressof(_Val));
        }

        const_pointer address(const_reference _Val) const _NOEXCEPT
        {	// return address of nonmutable _Val
            return (_STD addressof(_Val));
        }

        allocator() _THROW0()
        {	// construct default allocator (do nothing)
        }

        allocator(const allocator<double>&) _THROW0()
        {	// construct by copying (do nothing)
        }

        template<class _Other>
        allocator(const allocator<_Other>&) _THROW0()
        {	// construct from a related allocator (do nothing)
        }

        template<class _Other>
        allocator<double>& operator=(const allocator<_Other>&)
        {	// assign from a related allocator (do nothing)
            return (*this);
        }

        void deallocate(pointer _Ptr, size_type)
        {	// deallocate object at _Ptr, ignore size
            ::operator delete(_Ptr);
        }

        pointer allocate(size_type _Count)
        {	// allocate array of _Count elements
            return (_Allocate(_Count, (pointer)0));
        }

        pointer allocate(size_type _Count, const void *)
        {	// allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        void construct(double *_Ptr)
        {	// default construct object at _Ptr
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) double();
        }

        void construct(double *_Ptr, const double& _Val)
        {	// construct object at _Ptr with value _Val
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) double(_Val);
        }

#define _ALLOC_MEMBER_CONSTRUCT( \
	TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, CALL_OPT, X2, X3, X4) \
	template<class _Objty COMMA LIST(_CLASS_TYPE)> \
		void construct(_Objty *_Ptr COMMA LIST(_TYPE_REFREF_ARG)) \
		{	/* construct _Objty(_Types...) at _Ptr */ \
            PRINT_CONSTRUCT_CALL(__FUNCSIG__); \
            ::new ((void *)_Ptr) _Objty(LIST(_FORWARD_ARG)); \
		}

        _VARIADIC_EXPAND_0X(_ALLOC_MEMBER_CONSTRUCT, , , , )
#undef _ALLOC_MEMBER_CONSTRUCT

        template<class _Uty>
        void destroy(_Uty *_Ptr)
        {	// destroy object at _Ptr
            std::cout << __FUNCTION__ << std::endl;
            _Ptr->~_Uty();
        }

        size_t max_size() const _THROW0()
        {	// estimate maximum array size
            return ((size_t)(-1) / sizeof(double));
        }
    };

} // namespace std {

  // The simplest standards-conforming allocator
template<typename T>
class aligned_allocator
{	// generic aligned_allocator for type double
public:

  typedef T value_type;

  typedef value_type * pointer;
  typedef value_type const *const_pointer;

  template<class U>
  struct rebind
  {	// convert this type to allocator<_Other>
    typedef aligned_allocator<U> other;
  };

  aligned_allocator()
  {	// construct default aligned_allocator (do nothing)
  }

  aligned_allocator(aligned_allocator const &)
  {	// construct default aligned_allocator (do nothing)
  }

  template<typename U>
  aligned_allocator(aligned_allocator<U> const &)
  {	// construct default aligned_allocator (do nothing)
  }

  bool operator==(aligned_allocator const &) const { return true; }

  bool operator!=(aligned_allocator const &) const { return true; }

  pointer allocate(size_t size) const
  {	// allocate array of _Count elements
    return static_cast<T*>(::operator new(size * sizeof(T)));
  }

  void deallocate(pointer p, size_t) const
  {	// deallocate object at _Ptr, ignore size
    ::operator delete(p);
  }

  void construct(double *p)
  {	// default construct object at _Ptr
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double();
  }

  void construct(double *p, const double& val)
  {	// construct object at _Ptr with value _Val
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double(val);
  }

  template<class U>
  void destroy(U * p)
  {	// destroy object at _Ptr
    p->~U();
  }

};


#elif _MSC_VER == 1800 // VS2013


namespace std {

    // TEMPLATE CLASS allocator
    template<>
    class allocator<double>
        : public _Allocator_base<double>
    {	// generic allocator for objects of class double
    public:
        typedef allocator<double> other;

        typedef _Allocator_base<double> _Mybase;
        typedef _Mybase::value_type value_type;

        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef void *void_pointer;
        typedef const void *const_void_pointer;

        typedef value_type& reference;
        typedef const value_type& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef false_type propagate_on_container_copy_assignment;
        typedef false_type propagate_on_container_move_assignment;
        typedef false_type propagate_on_container_swap;

        allocator<double> select_on_container_copy_construction() const
        {	// return this allocator
            return (*this);
        }

        template<class _Other>
        struct rebind
        {	// convert this type to allocator<_Other>
            typedef allocator<_Other> other;
        };

        pointer address(reference _Val) const _NOEXCEPT
        {	// return address of mutable _Val
            return (_STD addressof(_Val));
        }

        const_pointer address(const_reference _Val) const _NOEXCEPT
        {	// return address of nonmutable _Val
            return (_STD addressof(_Val));
        }

        allocator() _THROW0()
        {	// construct default allocator (do nothing)
        }

        allocator(const allocator<double>&) _THROW0()
        {	// construct by copying (do nothing)
        }

        template<class _Other>
        allocator(const allocator<_Other>&) _THROW0()
        {	// construct from a related allocator (do nothing)
        }

        template<class _Other>
        allocator<double>& operator=(const allocator<_Other>&)
        {	// assign from a related allocator (do nothing)
            return (*this);
        }

        void deallocate(pointer _Ptr, size_type)
        {	// deallocate object at _Ptr, ignore size
            ::operator delete(_Ptr);
        }

        pointer allocate(size_type _Count)
        {	// allocate array of _Count elements
            return (_Allocate(_Count, (pointer)0));
        }

        pointer allocate(size_type _Count, const void *)
        {	// allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        void construct(double *_Ptr)
        {	// default construct object at _Ptr
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) double();
        }

        void construct(double *_Ptr, const double& _Val)
        {	// construct object at _Ptr with value _Val
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) double(_Val);
        }

        template<class _Objty,
            class... _Types>
            void construct(_Objty *_Ptr, _Types&&... _Args)
        {	// construct _Objty(_Types...) at _Ptr
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) _Objty(_STD forward<_Types>(_Args)...);
        }


        template<class _Uty>
        void destroy(_Uty *_Ptr)
        {	// destroy object at _Ptr
            _Ptr->~_Uty();
        }

        size_t max_size() const _THROW0()
        {	// estimate maximum array size
            return ((size_t)(-1) / sizeof(double));
        }
    };

} // namespace std {

// The simplest standards-conforming allocator
template<typename T>
class aligned_allocator
{	// generic aligned_allocator for type double
public:

  typedef T value_type;

  typedef value_type * pointer;
  typedef value_type const *const_pointer;

  aligned_allocator() 
  {	// construct default aligned_allocator (do nothing)
  }

  aligned_allocator(aligned_allocator const &) 
  {	// construct default aligned_allocator (do nothing)
  }

  template<typename U>
  aligned_allocator(aligned_allocator<U> const &) 
  {	// construct default aligned_allocator (do nothing)
  }

  bool operator==(aligned_allocator const &) const  { return true; }

  bool operator!=(aligned_allocator const &) const  { return true; }

  pointer allocate(size_t size) const
  {	// allocate array of _Count elements
    return static_cast<T*>(::operator new(size * sizeof(T)));
  }

  void deallocate(pointer p, size_t) const 
  {	// deallocate object at _Ptr, ignore size
    ::operator delete(p);
  }

  void construct(double *p)
  {	// default construct object at _Ptr
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double();
  }

  void construct(double *p, const double& val)
  {	// construct object at _Ptr with value _Val
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) double(val);
  }

  template<class U,
    class... Args>
    void construct(U * p, Args&&... args)
  {	// construct U(TArgs...) at _Ptr
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) U(_STD forward<Args>(args)...);
  }


  template<class U>
  void destroy(U * p)
  {	// destroy object at _Ptr
    p->~U();
  }

};


#elif _MSC_VER == 1900 // VS2015


namespace std {

    // TEMPLATE CLASS allocator
    template<>
    class allocator<double>
    {	// generic allocator for objects of class _Ty
    public:

        typedef void _Not_user_specialized;

        typedef double value_type;

        typedef value_type *pointer;
        typedef const value_type *const_pointer;

        typedef value_type& reference;
        typedef const value_type& const_reference;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef true_type propagate_on_container_move_assignment;
        typedef true_type is_always_equal;

        template<class _Other>
        struct rebind
        {	// convert this type to allocator<_Other>
            typedef allocator<_Other> other;
        };

        pointer address(reference _Val) const _NOEXCEPT
        {	// return address of mutable _Val
            return (_STD addressof(_Val));
        }

        const_pointer address(const_reference _Val) const _NOEXCEPT
        {	// return address of nonmutable _Val
            return (_STD addressof(_Val));
        }

        allocator() _THROW0()
        {	// construct default allocator (do nothing)
        }

        allocator(const allocator<double>&) _THROW0()
        {	// construct by copying (do nothing)
        }

        template<class _Other>
        allocator(const allocator<_Other>&) _THROW0()
        {	// construct from a related allocator (do nothing)
        }

        template<class _Other>
        allocator<double>& operator=(const allocator<_Other>&)
        {	// assign from a related allocator (do nothing)
            return (*this);
        }

        void deallocate(pointer _Ptr, size_type _Count)
        {	// deallocate object at _Ptr
            _Deallocate(_Ptr, _Count, sizeof(double));
        }

        _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count)
        {	// allocate array of _Count elements
            return (static_cast<pointer>(_Allocate(_Count, sizeof(double))));
        }

        _DECLSPEC_ALLOCATOR pointer allocate(size_type _Count, const void *)
        {	// allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        template<class _Objty,
            class... _Types>
            void construct(_Objty *_Ptr, _Types&&... _Args)
        {	// construct _Objty(_Types...) at _Ptr
            PRINT_CONSTRUCT_CALL(__FUNCSIG__);
            ::new ((void *)_Ptr) _Objty(_STD forward<_Types>(_Args)...);
        }


        template<class _Uty>
        void destroy(_Uty *_Ptr)
        {	// destroy object at _Ptr
            _Ptr->~_Uty();
        }

        size_t max_size() const _NOEXCEPT
        {	// estimate maximum array size
            return ((size_t)(-1) / sizeof(double));
        }
    };

} // namespace std {


// The simplest standards-conforming allocator
template<typename T>
class aligned_allocator
{	// generic aligned_allocator for type double
public:

  typedef T value_type;

  typedef value_type * pointer;
  typedef value_type const *const_pointer;

  aligned_allocator() noexcept
  {	// construct default aligned_allocator (do nothing)
  }

  aligned_allocator(aligned_allocator const &) noexcept
  {	// construct default aligned_allocator (do nothing)
  }

  template<typename U>
  aligned_allocator(aligned_allocator<U> const &) noexcept
  {	// construct default aligned_allocator (do nothing)
  }

  bool operator==(aligned_allocator const &) const noexcept { return true; }

  bool operator!=(aligned_allocator const &) const noexcept { return true; }

  __declspec(allocator) pointer allocate(size_t size) const
  {	// allocate array of _Count elements
    return static_cast<T*>(::operator new(size * sizeof(T)));
  }

  void deallocate(pointer p, size_t) const noexcept
  {	// deallocate object at _Ptr, ignore size
    ::operator delete(p);
  }

  template<class U,
    class... Args>
    void construct(U * p, Args&&... args)
  {	// construct U(TArgs...) at _Ptr
    PRINT_CONSTRUCT_CALL(__FUNCSIG__);
    ::new ((void *)p) U(_STD forward<Args>(args)...);
  }


  template<class U>
  void destroy(U * p)
  {	// destroy object at _Ptr
    p->~U();
  }

};

#else 

#error The MS _MSC_VER compiler is not surported!

#endif


template<typename Vector> 
void test()
{
  std::cout << "Vector<double> vec1(3);" << std::endl;
  Vector vec1(1);
  for (Vector::const_iterator i=vec1.begin() ; i!=vec1.end() ; ++i)
  {
    std::cout << " " << *i;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  {
    std::cout << "Vector<double> vec2(3,0.0);" << std::endl;
    Vector vec2(1, 0.0);
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2(vec1);" << std::endl;
    Vector vec2(vec1);
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2;" << std::endl;
    std::cout << "vec2 = vec1;" << std::endl;
    Vector vec2;
    vec2 = vec1;
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2(vec1.begin(), vec1.end());" << std::endl;
    Vector vec2(vec1.begin(), vec1.end());
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2;" << std::endl;
    std::cout << "vec2.assign(vec1.begin(), vec1.end());" << std::endl;
    Vector vec2;
    vec2.assign(vec1.begin(), vec1.end());
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2(vec1.data(), vec1.data()+ vec1.size());" << std::endl;
    Vector vec2(&vec1.front(), &vec1.front() + vec1.size());
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

  {
    std::cout << "Vector<double> vec2;" << std::endl;
    std::cout << "vec2.assign(vec1.data(), vec1.data()+ vec1.size());" << std::endl;
    Vector vec2;
    vec2.assign(&vec1.front(), &vec1.front() + vec1.size());
    for (Vector::const_iterator i=vec2.begin() ; i!=vec2.end() ; ++i)
    {
      std::cout << " " << *i;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

}


int main()
{
    std::cout << "_MSC_VER = " << _MSC_VER << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    {
      std::cout << "********************************************************" << std::endl;
      std::cout << "Test                                      std::vector<T>" << std::endl;
      std::cout << "********************************************************" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;

      test<std::vector<double>>();

      std::cout << "********************************************************" << std::endl;
      std::cout << "Test      std::vector<double,aligned_allocator<double>>>" << std::endl;
      std::cout << "********************************************************" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;

      test<std::vector<double,aligned_allocator<double>>>();
    }

    std::cout << std::endl << "Finished...." << std::endl;
    return 0;
}

