#ifndef __SHARED_UTILS_COMMAND_BASE_H__
#define __SHARED_UTILS_COMMAND_BASE_H__

#include "Utils.h"

class CommandBase
{
public:
	CommandBase() {}
	virtual ~CommandBase();

	virtual void Run() = 0;

private:
	DISALLOW_EVIL_CONSTRUCTORS(CommandBase);
};

class FunctionClosure0 : public CommandBase 
{
public:
	typedef void (*FunctionType)();

	FunctionClosure0(FunctionType function, bool self_deleting)
		: function_(function), self_deleting_(self_deleting) {}
	~FunctionClosure0();

	void Run()
	{
		bool needs_delete = self_deleting_;  // read in case callback deletes
		function_();
		if (needs_delete) delete this;
	}

private:
	FunctionType function_;
	bool self_deleting_;
};

template <typename Class>
class MethodClosure0 : public CommandBase 
{
public:
	typedef void (Class::*MethodType)();

	MethodClosure0(Class* object, MethodType method, bool self_deleting)
		: object_(object), method_(method), self_deleting_(self_deleting) 
	{

	}
	~MethodClosure0() 
	{

	}

	void Run() 
	{
		bool needs_delete = self_deleting_;  // read in case callback deletes
		(object_->*method_)();
		if (needs_delete) delete this;
	}

private:
	Class* object_;
	MethodType method_;
	bool self_deleting_;
};

template <typename Arg1>
class FunctionClosure1 : public CommandBase 
{
public:
	typedef void (*FunctionType)(Arg1 arg1);

	FunctionClosure1(FunctionType function, bool self_deleting,
		Arg1 arg1)
		: function_(function), self_deleting_(self_deleting),
		arg1_(arg1) {}
	~FunctionClosure1() {}

	void Run() {
		bool needs_delete = self_deleting_;  // read in case callback deletes
		function_(arg1_);
		if (needs_delete) delete this;
	}

private:
	FunctionType function_;
	bool self_deleting_;
	Arg1 arg1_;
};

template <typename Class, typename Arg1>
class MethodClosure1 : public CommandBase 
{
public:
	typedef void (Class::*MethodType)(Arg1 arg1);

	MethodClosure1(Class* object, MethodType method, bool self_deleting,
		Arg1 arg1)
		: object_(object), method_(method), self_deleting_(self_deleting),
		arg1_(arg1) {}
	~MethodClosure1() {}

	void Run() 
	{
		bool needs_delete = self_deleting_;  // read in case callback deletes
		(object_->*method_)(arg1_);
		if (needs_delete) delete this;
	}

private:
	Class* object_;
	MethodType method_;
	bool self_deleting_;
	Arg1 arg1_;
};

template <typename Arg1, typename Arg2>
class FunctionClosure2 : public CommandBase 
{
public:
	typedef void (*FunctionType)(Arg1 arg1, Arg2 arg2);

	FunctionClosure2(FunctionType function, bool self_deleting,
		Arg1 arg1, Arg2 arg2)
		: function_(function), self_deleting_(self_deleting),
		arg1_(arg1), arg2_(arg2) {}
	~FunctionClosure2() {}

	void Run() 
	{
		bool needs_delete = self_deleting_;  // read in case callback deletes
		function_(arg1_, arg2_);
		if (needs_delete) delete this;
	}

private:
	FunctionType function_;
	bool self_deleting_;
	Arg1 arg1_;
	Arg2 arg2_;
};

template <typename Class, typename Arg1, typename Arg2>
class MethodClosure2 : public CommandBase 
{
public:
	typedef void (Class::*MethodType)(Arg1 arg1, Arg2 arg2);

	MethodClosure2(Class* object, MethodType method, bool self_deleting,
		Arg1 arg1, Arg2 arg2)
		: object_(object), method_(method), self_deleting_(self_deleting),
		arg1_(arg1), arg2_(arg2) {}
	~MethodClosure2() {}

	void Run() 
	{
		bool needs_delete = self_deleting_;  // read in case callback deletes
		(object_->*method_)(arg1_, arg2_);
		if (needs_delete) delete this;
	}

private:
	Class* object_;
	MethodType method_;
	bool self_deleting_;
	Arg1 arg1_;
	Arg2 arg2_;
};


// See Closure.
inline CommandBase* NewCallback(void (*function)()) 
{
	return new FunctionClosure0(function, true);
}

// See Closure.
inline CommandBase* NewPermanentCallback(void (*function)()) 
{
	return new FunctionClosure0(function, false);
}

// See Closure.
template <typename Class>
inline CommandBase* NewCallback(Class* object, void (Class::*method)()) 
{
	return new MethodClosure0<Class>(object, method, true);
}

// See Closure.
template <typename Class>
inline CommandBase* NewPermanentCallback(Class* object, void (Class::*method)()) 
{
	return new MethodClosure0<Class>(object, method, false);
}

// See Closure.
template <typename Arg1>
inline CommandBase* NewCallback(void (*function)(Arg1), Arg1 arg1) 
{
	return new FunctionClosure1<Arg1>(function, true, arg1);
}

// See Closure.
template <typename Arg1>
inline CommandBase* NewPermanentCallback(void (*function)(Arg1), Arg1 arg1) 
{
	return new FunctionClosure1<Arg1>(function, false, arg1);
}

// See Closure.
template <typename Class, typename Arg1>
inline CommandBase* NewCallback(Class* object, void (Class::*method)(Arg1), Arg1 arg1) 
{
	return new MethodClosure1<Class, Arg1>(object, method, true, arg1);
}

// See Closure.
template <typename Class, typename Arg1>
inline CommandBase* NewPermanentCallback(Class* object, void (Class::*method)(Arg1), Arg1 arg1)
{
	return new MethodClosure1<Class, Arg1>(object, method, false, arg1);
}

// See Closure.
template <typename Arg1, typename Arg2>
inline CommandBase* NewCallback(void (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) 
{
	return new FunctionClosure2<Arg1, Arg2>(function, true, arg1, arg2);
}

// See Closure.
template <typename Arg1, typename Arg2>
inline CommandBase* NewPermanentCallback(void (*function)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) 
{
	return new FunctionClosure2<Arg1, Arg2>(function, false, arg1, arg2);
}

// See Closure.
template <typename Class, typename Arg1, typename Arg2>
inline CommandBase* NewCallback(Class* object, void (Class::*method)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) 
{
	return new MethodClosure2<Class, Arg1, Arg2>(object, method, true, arg1, arg2);
}

// See Closure.
template <typename Class, typename Arg1, typename Arg2>
inline CommandBase* NewPermanentCallback(Class* object, void (Class::*method)(Arg1, Arg2),Arg1 arg1, Arg2 arg2)
{
	return new MethodClosure2<Class, Arg1, Arg2>(object, method, false, arg1, arg2);
}

#endif
