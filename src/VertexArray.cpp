#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	size_t offset = 0;
	for (size_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i,element.count,element.type,element.normalized,layout.getStride(),reinterpret_cast<const void*>(offset)));
		offset += element.count * VertexBufferElement::GetSIzeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}