#include "Containers.hpp"

Containers::Mail::Mail(Person& sender, vector<pair<Person*, Receiver>>& receivers, const string& content, const Headers& headers, const Date& sendDate):
sender(&sender), receivers(receivers), content(content), headers(headers), sendDate(sendDate)
{
    this->complete_cycle = false;
    this->part_of_a_cycle = false;
}

void Containers::Mail::addAttachment(const Attachment& attachment)
{
	this->attachments.push_back(attachment);
}

const vector<Containers::Attachment>& Containers::Mail::getAttachments() const
{
	return this->attachments;
}
