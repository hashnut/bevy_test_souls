/**
 * 스마트 포인터 활용을 용이하게 하기 위한 풀
 * MakeShared 로 TSharedRef 를 받고, RemovePool 로 메모리를 해제한다.
 *
 * 1. TQueue 사용으로 Resize 등에 대한 비용 없음
 * 2. TSharedRef 사용시 불필요한 nullptr 체크 없음
 * 3. TSharedRef 사용시 풀에 수동으로 반납할 필요 없음 (커스텀 삭제자 적용)
*/
template <typename Type>
class TGsSmartPool final
{
    static_assert(!TIsDerivedFrom<Type, UObject>::IsDerived, "Type is UObject! Use TGsUObjectPool instead");

private:
    TQueue<Type*> Pool;

public:
    TGsSmartPool() = default;
    ~TGsSmartPool()
    {
        RemovePool();
    }

public:
    /** 생 포인터 객체가 필요하면 Pop 을 사용한다 */
    template<typename... Args>
    Type* Pop(Args&&... args)
    {
        Type* memory = nullptr;
        if (!Pool.Dequeue(memory))
        {
            memory = reinterpret_cast<Type*>(FMemory::Malloc(sizeof(Type)));
        }

        new(memory)Type(std::forward<Args>(args)...);
        return memory;
    }

    /** 풀에 객체를 반납한다 */
    void Push(Type* InObj)
    {
        InObj->~Type();
        Pool.Enqueue(InObj);
    }

    /** TSharedRef 를 이용해 객체를 관리한다. 객체가 소멸될 때 풀로 되돌아간다 */
    template<typename... Args>
    TSharedRef<Type> MakeShared(Args&&...args)
    {
        Type* obj = Pop(std::forward<Args>(args)...);
        TSharedRef<Type> sharedRef = MakeShareable(obj, [=](Type*) { Push(obj); });
        return MoveTemp(sharedRef);
    }

    /** 풀 자체를 관리하는 클래스가 있다면, 사용완료 시 명시적으로 풀을 비워줘도 된다 */
    void RemovePool()
    {
        Type* data = nullptr;
        while (Pool.Dequeue(data))
        {
            FMemory::Free(data);
            data = nullptr;
        }
    }
};



/**
 * 스마트 포인터 활용을 용이하게 하기 위한 풀 (TArray 버전)
 * MakeShared 로 TSharedRef 를 받고, RemovePool 로 메모리를 해제한다.
 *
 * 1. 풀을 통해 관리할 객체의 갯수를 미리 예측할 수 있을 때 유용함
 * 2. TSharedRef 사용시 불필요한 nullptr 체크 없음
 * 3. TSharedRef 사용시 풀에 수동으로 반납할 필요 없음 (커스텀 삭제자 적용)
*/

#define DEFAULT_SMART_PRE_SIZE_POOL_SIZE 4

template <typename Type>
class TGsSmartPreSizePool final
{
    static_assert(!TIsDerivedFrom<Type, UObject>::IsDerived, "Type is UObject! Use TGsUObjectPool instead");

private:
    TArray<Type*> Pool;

    /** 현재 대여 가능한 Element 의 Index */
    int32 UseIndex = -1;

public:
    TGsSmartPreSizePool()
    {
        InitPool();
    }

    TGsSmartPreSizePool(int32 InSize)
    {
        InitPool(InSize < 1 ? 1 : InSize);
    }
    ~TGsSmartPreSizePool()
    {
        RemovePool();
    }

    void InitPool(int32 InSize = DEFAULT_SMART_PRE_SIZE_POOL_SIZE)
    {
        Pool.Empty(InSize);
        for (int32 i = 0; i < InSize; ++i)
        {
            Pool.Emplace(reinterpret_cast<Type*>(FMemory::Malloc(sizeof(Type))));
        }
        UseIndex = 0;
    }

public:
    /** 생 포인터 객체가 필요한 어쩔 수 없는 경우, Pop 을 사용한다. */
    template<typename... Args>
    Type* Pop(Args&&... args)
    {
        Type* memory = nullptr;
        if (UseIndex > -1 && UseIndex < Pool.Num())
        {
            memory = Pool[UseIndex];
        }
        else
        {
            memory = reinterpret_cast<Type*>(FMemory::Malloc(sizeof(Type)));
        }

        new(memory)Type(std::forward<Args>(args)...);

        // 풀을 사용함
        if (UseIndex > -1 && UseIndex < Pool.Num())
        {
            ++UseIndex;
        }
        // 풀이 비어 있거나 꽉 참
        else
        {
            Pool.Emplace(memory);
            UseIndex = Pool.Num();
        }

        return memory;
    }

    /** 풀에 객체를 반납한다 */
    void Push(Type* InObj)
    {
        InObj->~Type();
        --UseIndex;
    }

    /** TSharedRef 를 이용해 객체를 관리한다. 객체가 소멸될 때 풀로 되돌아간다 */
    template<typename... Args>
    TSharedRef<Type> MakeShared(Args&&...args)
    {
        Type* obj = Pop(std::forward<Args>(args)...);
        TSharedRef<Type> sharedRef = MakeShareable(obj, [=](Type*) { Push(obj); });
        return MoveTemp(sharedRef);
    }

    /** 풀 자체를 관리하는 클래스가 있다면, 사용완료 시 명시적으로 풀을 비워줘도 된다 */
    void RemovePool()
    {
        for (Type* data : Pool)
        {
            FMemory::Free(data);
            data = nullptr;
        }
        Pool.Empty();

        // 풀 재활용을 염두에 둔 작업이다.
        UseIndex = -1;
    }
};