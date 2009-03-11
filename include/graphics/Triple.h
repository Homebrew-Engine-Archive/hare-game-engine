namespace hare_graphics
{
	class Triple
	{
	public:
		Triple();
		virtual ~Triple();

		virtual Vertex* getBuffer();
		virtual uInt    getVertexCount();
		virtual uInt    getPrimCount();
		virtual bool    isUseIndex();
		virtual uInt    getOperationType();
	private:
		Vertex buffer[3];
	};
}