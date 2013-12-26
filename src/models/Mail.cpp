#include "Containers.hpp"

Containers::Mail::Mail(Person& sender, Person& receiver, const string& content, const Headers& headers, const Date& sendDate):
sender(&sender), receiver(&receiver), content(content), headers(headers), sendDate(sendDate)
{
	
}

void Containers::Mail::addAttachment(const Attachment& attachment)
{
	this->attachments.push_back(attachment);
}

const vector<Containers::Attachment>& Containers::Mail::getAttachments() const
{
	return this->attachments;
}
