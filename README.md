# CMPS2131 — Lab 1: Token Linked List
Write Up:
When a token is successfully added to the list, the list owns it. A token starts in token_make, which puts a copy 
the text on the heap. So the token owns the string. When we call list_push_back and it runs , ownership moves
to the list. List_destory walks through every node and frees the string with toen_free, then frees the node.
The same process occurs when running list_remove_first.If push_back runs out of memory and cannot add the token,
we free it immediately.The string stays allocated forever and becomes a memory leak.Valgrind would report it and the list
would take ownership only when the push succeeds.
