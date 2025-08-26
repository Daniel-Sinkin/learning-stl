#include <new>
#include <format>
#include <stdexcept>
#include <memory>

namespace DS::List {
class ListNode {
public:
	explicit ListNode(int value) : value_(value) {}

	void append_value(int value) {
		auto node = std::make_unique<ListNode>(value);

		node->prev_ = this;
		node->next_ = std::move(next_);
		if(node->next_) node->next_->prev_ = node.get();
		next_ = std::move(node);
	}

	void prepend_value(int value) {
		if(!prev_) {
			throw std::logic_error("prepend before head must be done by the list");
		}
		auto node = std::make_unique<ListNode>(value);
		node->prev_ = prev_;
		node->next_ = std::move(prev_next_);
		node->next_->prev_ = node.get();
		prev_->next_ = std::move(node);
		prev_ = prev_->next_.get();
	}

private:
	ListNode* prev_ = nullptr;
	std::unique_ptr<ListNode> next_ = nullptr;
	int value_;
};

} // DS::List 
