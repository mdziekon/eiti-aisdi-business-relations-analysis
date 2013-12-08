#include "Containers.hpp"

Containers::Mail::Mail(Person& sender, Person& receiver, const string& content, const Headers& headers, const int& sendTimestap):
sender(&sender), receiver(&receiver), content(content), headers(headers), sendTimestamp(sendTimestap)
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
