#include <new>
#include <format>
#include <stdexcept>
#include <memory>

// Forward declaration to make std::format friend
namespace DS::List { class ListNode; }
namespace std {
    template<> struct formatter<DS::List::ListNode>;
}

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

		++num_forward_;
	}

	void prepend_value(int value) {
		if(!prev_) {
			throw std::logic_error("prepend before head must be done by the list");
		}
		auto node = std::make_unique<ListNode>(value);
		node->prev_ = prev_;
		node->next_ = std::move(prev_->next_);
		node->next_->prev_ = node.get();
		prev_->next_ = std::move(node);
		prev_ = prev_->next_.get();

		++num_backward_;
	}

	void append(ListNode& node) {
		auto begin = node.get_begin();
		auto end = node.get_end();

		num_forward_ += node.get_length();

		next_->prev_ = end;
		end.next_ = std::move(next_);
		begin.prev_ = this;

		next_ = begin;
		begin.prev_ = this;
	}

	[[nodiscard]] ListNode& get_end() {
		ListNode* curr = this;
		while(curr->next_) curr = curr->next_.get();
		return *curr;
	}

	[[nodiscard]] const ListNode& get_end() const {
		const ListNode* curr = this;
		while(curr->next_) curr = curr->next_.get();
		return *curr;
	}

	[[nodiscard]] ListNode& get_begin() {
		ListNode* curr = this;
		while(curr->prev_) curr = curr->prev_;
		return *curr;
	}

	[[nodiscard]] const ListNode& get_begin() const {
		const ListNode* curr = this;
		while(curr->prev_) curr = curr->prev_;
		return *curr;
	}

	[[nodiscard]] int value() const { return value_; }
	[[nodiscard]] const ListNode* next() const { return next_.get(); }
	[[nodiscard]] ListNode* next() { return next_.get(); }
	[[nodiscard]] const ListNode* prev() const { return prev_; }
	[[nodiscard]] ListNode* prev() { return prev_; }
	[[nodiscard]] const ListNode& head() const { return get_begin(); }
	[[nodiscard]] ListNode& head() { return get_begin(); }

	[[nodiscard]] size_t get_num_forward() { return num_forward_; }
	[[nodiscard]] size_t get_num_backward() { return num_backward_; }

	[[nodiscard]] size_t get_length() {
		return num_backward_ + 1 + num_foward_;
	}
private:
	ListNode* prev_ = nullptr;
	std::unique_ptr<ListNode> next_ = nullptr;
	int value_;

	size_t num_forward_;
	size_t num_backward_;
};
} // DS::List 


template <>
struct std::formatter<DS::List::ListNode> {
    // no custom format specifiers
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const DS::List::ListNode& node, FormatContext& ctx) const {
        auto out = ctx.out();
        auto* head = &node.get_begin();
        auto* curr = &node;

        out = std::format_to(out, "[");
        bool first = true;
        for (auto* n = head; n != nullptr; n = n->next()) {
            if (!first) {
                out = std::format_to(out, ", ");
            }
            if (n == curr) {
                out = std::format_to(out, "({})", n->value());
            } else {
                out = std::format_to(out, "{}", n->value());
            }
            first = false;
        }
        out = std::format_to(out, "]");
        return out;
    }
};


