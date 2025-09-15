using System;

public class VoiceChatCircularBuffer<T>
{
	private int capacity;

	private int count;

	private int head;

	private int tail;

	private T[] buffer;

	public bool HasItems => count > 0;

	public int TailIndex => tail;

	public T[] Data => buffer;

	public int Count => count;

	public int Capacity => capacity;

	public VoiceChatCircularBuffer(int maxCapacity)
	{
		capacity = maxCapacity;
		buffer = new T[capacity];
	}

	public void Add(T item)
	{
		if (count == capacity && head == tail && ++tail == capacity)
		{
			tail = 0;
		}
		buffer[head] = item;
		if (++head == capacity)
		{
			head = 0;
		}
		count = Math.Min(capacity, count + 1);
	}

	public T Remove()
	{
		if (count == 0)
		{
			throw new ArgumentOutOfRangeException();
		}
		T result = buffer[tail];
		if (++tail == capacity)
		{
			tail = 0;
		}
		count--;
		return result;
	}

	public bool NextIndex(ref int value)
	{
		if (++value == capacity)
		{
			value = 0;
		}
		return value != head;
	}
}
