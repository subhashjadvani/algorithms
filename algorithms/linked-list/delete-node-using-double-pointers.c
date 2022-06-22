// Sources: https://stackoverflow.com/questions/12914917/using-pointers-to-remove-item-from-singly-linked-list
// https://www.reddit.com/r/HandmadeQuake/comments/4d6993/linus_torvaldss_double_pointer_problem/?utm_medium=android_app&utm_source=share

typedef struct nodetype
{
    struct nodetype * next;
} node;

typedef struct listType
{
    struct nodetype * head;
} list;

void remove_node(list * l, node * n)
{
   node ** p = &(l->head);

   while(* p != n)
   {
       p = &(* p)->next;
   }
   * p = (* p)->next;
}
