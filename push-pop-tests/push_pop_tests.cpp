#include <iostream>

#include <catch2/catch_test_macros.hpp>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

TEST_CASE("PushBack adds element to empty list", "[list][pushback]")
{
    List list;
    REQUIRE(list.Size() == 0);

    list.PushBack(10);

    REQUIRE(list.Size() == 1);
    REQUIRE(list.Empty() == false);
}

TEST_CASE("PushBack adds multiple elements", "[list][pushback]")
{
    List list;

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);

    REQUIRE(list.Size() == 3);
}

TEST_CASE("PushFront adds element to empty list", "[list][pushfront]")
{
    List list;
    REQUIRE(list.Size() == 0);

    list.PushFront(42);

    REQUIRE(list.Size() == 1);
    REQUIRE(list.Empty() == false);
}

TEST_CASE("PushFront adds multiple elements", "[list][pushfront]")
{
    List list;

    list.PushFront(10);
    list.PushFront(20);
    list.PushFront(30);

    REQUIRE(list.Size() == 3);
}

TEST_CASE("PopBack removes and returns last element", "[list][popback]")
{
    List list;
    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    int value = list.PopBack();

    REQUIRE(value == 30);
    REQUIRE(list.Size() == 2);
}

TEST_CASE("PopBack on empty list throws exception", "[list][popback][exception]")
{
    List list;
    REQUIRE(list.Empty());

    REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
}

TEST_CASE("PopBack works until list is empty", "[list][popback]")
{
    List list;
    list.PushBack(5);
    list.PushBack(15);

    REQUIRE(list.PopBack() == 15);
    REQUIRE(list.Size() == 1);

    REQUIRE(list.PopBack() == 5);
    REQUIRE(list.Size() == 0);
    REQUIRE(list.Empty());
}

TEST_CASE("PopFront removes and returns first element", "[list][popfront]")
{
    List list;
    list.PushBack(10);
    list.PushBack(20);
    list.PushBack(30);

    int value = list.PopFront();

    REQUIRE(value == 10);
    REQUIRE(list.Size() == 2);
}

TEST_CASE("PopFront on empty list throws exception", "[list][popfront][exception]")
{
    List list;
    REQUIRE(list.Empty());

    REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
}

TEST_CASE("PopFront works until list is empty", "[list][popfront]")
{
    List list;
    list.PushBack(5);
    list.PushBack(15);

    REQUIRE(list.PopFront() == 5);
    REQUIRE(list.Size() == 1);

    REQUIRE(list.PopFront() == 15);
    REQUIRE(list.Size() == 0);
    REQUIRE(list.Empty());
}

SCENARIO("Alternating Push and Pop operations", "[list][integration]")
{
    GIVEN("An empty list")
    {
        List list;

        WHEN("I do PushBack, PushFront, PopBack, PushFront")
        {
            list.PushBack(10);
            list.PushFront(5);
            REQUIRE(list.Size() == 2);

            int removed = list.PopBack();   
            list.PushFront(1);

            THEN("The remaining elements are 1 and 5")
            {
                REQUIRE(list.Size() == 2);
                REQUIRE(removed == 10);
            }

            AND_WHEN("I remove both remaining elements")
            {
                int first = list.PopFront(); 
                int second = list.PopFront(); 

                THEN("The list is empty and values are correct")
                {
                    REQUIRE(first == 1);
                    REQUIRE(second == 5);
                    REQUIRE(list.Empty());
                    REQUIRE(list.Size() == 0);
                }
            }
        }
    }
}