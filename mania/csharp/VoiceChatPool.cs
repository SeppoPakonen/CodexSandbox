using System.Collections.Generic;

public abstract class VoiceChatPool<T> where T : class
{
	private Queue<T> queue = new Queue<T>();

	public T Get()
	{
		if (queue.Count > 0)
		{
			return queue.Dequeue();
		}
		return Create();
	}

	public void Return(T obj)
	{
		if (obj != null)
		{
			queue.Enqueue(obj);
		}
	}

	protected abstract T Create();
}
