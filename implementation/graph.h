#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <vector>

namespace graph {

	/* Forward declarations */
	template <class PayloadNode, class PayloadLink> class Link;
	template <class PayloadNode, class PayloadLink> class Node;

	/* Wrapper type for std::shared_ptr<Link<...>> */
	template <class PayloadNode, class PayloadLink>
	using LinkPtr = std::shared_ptr<Link<PayloadNode, PayloadLink> >;

	/* Wrapper type for std::shared_ptr<Node<...>> */
	template <class PayloadNode, class PayloadLink>
	using NodePtr = std::shared_ptr<Node<PayloadNode, PayloadLink> >;

	template<class PayloadNode, class PayloadLink>
	class Node : public PayloadNode {

		/* Payload from Base (Mixin) */

		typedef NodePtr<PayloadNode, PayloadLink> NodePtr;
		typedef LinkPtr<PayloadNode, PayloadLink> LinkPtr;

		std::vector<LinkPtr> ptrs_in;	/* incoming links */
		std::vector<LinkPtr> ptrs_out;	/* outgoing links */

	public:
		Node() {}

		Node(std::vector<LinkPtr> p_ptrs_in, std::vector<LinkPtr> p_ptrs_out)
		{
			ptrs_in  = p_ptrs_in;
			ptrs_out = p_ptrs_out;
		}

		~Node()
		{
			/* smartpointers will do the job */
		}

		void AddIncomingLink(LinkPtr p_link_in)
		{
			ptrs_in.push_back(p_link_in);
		}

		void AddOutgoingLink(LinkPtr p_link_out)
		{
			ptrs_out.push_back(p_link_out);
		}

		int GetIncomingVectorSize() const
		{
			return ptrs_in.size();
		}

		int GetOutgoingVectorSize() const
		{
			return ptrs_out.size();
		}

		LinkPtr GetIncomingLinkById(int p_id)
		{
			return ptrs_in[p_id];
		}

		LinkPtr GetOutgoingLinkById(int p_id)
		{
			return ptrs_out[p_id];
		}

		NodePtr GetParentNodeById(int p_id)
		{
			return GetIncomingLinkById(p_id)->GetNodeFrom();
		}

		NodePtr GetChildNodeById(int p_id)
		{
			return GetOutgoingLinkById(p_id)->GetNodeTo();
		}

		std::vector<LinkPtr> GetIncomingLinks()
		{
			return ptrs_in;
		}

		std::vector<LinkPtr> GetOutgoingLinks()
		{
			return ptrs_out;
		}

	};

	template <class PayloadNode, class PayloadLink>
	class Link : public PayloadLink {

		/* Payload from Base (Mixin) */

		typedef NodePtr<PayloadNode, PayloadLink> NodePtr;

		NodePtr from_ptr, to_ptr;

	public:
		Link() {}

		Link(NodePtr p_node_from, NodePtr p_node_to)
		{
			from_ptr = p_node_from;
			to_ptr   = p_node_to;
		}

		~Link()
		{
			/* smartpointers will do the job */
		}

		void SetNodeFrom(NodePtr p_node_from)
		{
			from_ptr = p_node_from;
		}

		void SetNodeTo(NodePtr p_node_to)
		{
			to_ptr = p_node_to;
		}

		NodePtr GetNodeFrom()
		{
			return from_ptr;
		}

		NodePtr GetNodeTo()
		{
			return to_ptr;
		}
	};

	/* HELPER FUNCTIONS */

	/* Wrapper for std::make_shared */
	template <class PayloadNode, class PayloadLink>
	std::shared_ptr<Node<PayloadNode, PayloadLink> > MakeNodePtr()
	{
		return std::make_shared<Node<PayloadNode, PayloadLink> >();
	}

	/* Wrapper for std::make_shared */
	template <class PayloadNode, class PayloadLink>
	std::shared_ptr<Link<PayloadNode, PayloadLink> > MakeLinkPtr()
	{
		return std::make_shared<Link<PayloadNode, PayloadLink> >();
	}

	template <class PayloadNode, class PayloadLink>
	void ConnectNodes(NodePtr<PayloadNode, PayloadLink> node_parent,
		NodePtr<PayloadNode, PayloadLink> node_child,
		LinkPtr<PayloadNode, PayloadLink> link)
	{
		link->SetNodeFrom(node_parent);
		link->SetNodeTo(node_child);

		node_parent->AddOutgoingLink(link);
		node_child->AddIncomingLink(link);
	}

}
#endif /* GRAPH_H */