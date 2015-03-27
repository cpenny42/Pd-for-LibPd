//
//  argv.c
//  
//
//  Created by Chris Penny on 3/27/15.
//
//

#include "m_pd.h"
#include "g_canvas.h"

#define IS_A_FLOAT(atom,index) ((atom+index)->a_type == A_FLOAT)
#define IS_A_SYMBOL(atom,index) ((atom+index)->a_type == A_SYMBOL)


static t_class *argv_class;

typedef struct _argv
{
    t_object    x_obj;
    void        *x_outlet_ac;
    t_atom      *x_at;
    int         x_ac;
} t_argv;

static void argv_float(t_argv *x, t_floatarg f)
{
    int i = (int)f;
    
    if(!i)
    {
        outlet_float(x->x_outlet_ac, x->x_ac);
        if(x->x_ac)
            outlet_list(x->x_obj.ob_outlet, &s_list, x->x_ac, x->x_at);
    }
    else if(i > 0)
    {
        if(i <= x->x_ac)
        {
            outlet_float(x->x_outlet_ac, i);
            if(IS_A_FLOAT(x->x_at, i-1))
                outlet_float(x->x_obj.ob_outlet, atom_getfloatarg(i-1, x->x_ac, x->x_at));
            else if(IS_A_SYMBOL(x->x_at, i-1))
                outlet_symbol(x->x_obj.ob_outlet, atom_getsymbolarg(i-1, x->x_ac, x->x_at));
        }
        else
            outlet_float(x->x_outlet_ac, 0);
    }
    else
    {
        int j = x->x_ac + i;
        
        if(j >= 0)
        {
            outlet_float(x->x_outlet_ac, j+1);
            if(IS_A_FLOAT(x->x_at, j))
                outlet_float(x->x_obj.ob_outlet, atom_getfloatarg(j, x->x_ac, x->x_at));
            else if(IS_A_SYMBOL(x->x_at, j))
                outlet_symbol(x->x_obj.ob_outlet, atom_getsymbolarg(j, x->x_ac, x->x_at));
        }
        else
            outlet_float(x->x_outlet_ac, 0);
    }
}

static void argv_bang( t_argv *x)
{
    argv_float(x, 0.0f);
}

static void argv_free(t_argv *x)
{
    if(x->x_ac) {
        freebytes(x->x_at, x->x_ac * sizeof(t_atom));
    }
}

static void *argv_new(void)
{
    t_argv *x = (t_argv *)pd_new(argv_class);
    t_glist *glist=(t_glist *)canvas_getcurrent();
    t_canvas *canvas=glist_getcanvas(glist);
    int pargc;
    t_atom *pargv, *at;
    
    canvas_setcurrent(canvas);
    canvas_getargs(&pargc, &pargv);
    canvas_unsetcurrent(canvas);
    x->x_at = (t_atom *)getbytes(pargc*sizeof(t_atom));
    x->x_ac = pargc;
    at = x->x_at;
    while(pargc--)
        *at++ = *pargv++;
    outlet_new(&x->x_obj, &s_list);
    x->x_outlet_ac = outlet_new(&x->x_obj, &s_float);
    return (x);
}

void argv_setup(void)
{
    argv_class = class_new(gensym("argv"), (t_newmethod)argv_new,
                              (t_method)argv_free, sizeof(t_argv), 0, 0);
    class_addcreator((t_newmethod)argv_new, gensym("$n"), 0);
    class_addbang(argv_class, (t_method)argv_bang);
    class_addfloat(argv_class, (t_method)argv_float);
}