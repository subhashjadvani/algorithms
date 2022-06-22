// Sources: https://stackoverflow.com/questions/12914917/using-pointers-to-remove-item-from-singly-linked-list

typedef struct nodetype
{
    struct nodetype * next;
}node;
typedef struct listType
{
    struct nodetype * head;
}list;

void remove_node(list * l, node * n)
{
   node ** p = &(l->head);

   while(* p != n)
   {
       p = &(* p)->next;
   }
   * p = (* p)->next;
}
