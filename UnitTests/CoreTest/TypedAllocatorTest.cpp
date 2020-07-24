#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Angazi::Core;

namespace CoreTest
{
	TEST_CLASS(TypedAllocatorTest)
	{
	public:
		class Bar // Used to verify the correct constructor being called
		{
		public:
			static int counter;
			Bar() { counter = 0; }
			Bar(const Bar&) { counter = 1; }
			Bar(Bar&&) { counter = 2; }
		};

		class Foo
		{
		public:
			int a, b;
			Foo() : a(42), b(7) {}
			Foo(int i, int j) : a(i), b(i) {}
			Foo(Bar b) : a(12), b(34) {}
			Foo(std::string str) : a(12), b(34) {}
			~Foo() { a = 0xfeeefeee, b = 0xdeadbeef; }
		};
	public:
		TEST_METHOD(OneBlockTest)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->a == 42);
			Assert::IsTrue(ptr->b == 7);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNull(ptr2);
		}

		TEST_METHOD(TestDelete)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr);
			Assert::AreEqual((uint32_t)ptr->a, 0xfeeefeee);
			Assert::AreEqual((uint32_t)ptr->b, 0xdeadbeef);
		}
		TEST_METHOD(TestRecycling)
		{
			TypedAllocator<Foo> typedAllocator(1);
			Foo* ptr = typedAllocator.New();
			Assert::IsNotNull(ptr);

			typedAllocator.Delete(ptr);

			Foo* ptr2 = typedAllocator.New();
			Assert::IsNotNull(ptr2);
			Assert::IsTrue(ptr == ptr2);
		}
	};
}
