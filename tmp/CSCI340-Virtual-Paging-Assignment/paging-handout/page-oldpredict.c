/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

	/* This file contains the stub for a predictive pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time

	/* Local vars */

	/* initialize static vars on first run */
	if (!initialized) {
		/* Init complex static vars here */

		initialized = 1;
	}
    int proc;
    int pc;
    int page;
    int futurepage;
    for(proc=0; proc < MAXPROCESSES; proc++)
    {
        if(q[proc].active)
        {
            pc = q[proc].pc;
            page = pc/PAGESIZE;
            switch(page)
            {
                case 0:
                    futurepage = 10;
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page+2; i < 20; i++)
                    {
                        if(q[proc].pages[i] == 1)
                        {
                            if(i != futurepage)
                                    pageout(proc,i);
                        }
                    }
                    break;
                case 1:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = (page+2); i < 20; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    break;
                case 2:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    
                    break;
                case 3:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 4:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 5:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 6:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 7:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 8:
                    futurepage = 10;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 9:
                    futurepage = 13;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 10:
                    futurepage = 13;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 11:
                    futurepage = 13;
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,futurepage);
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 12:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 13:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 14:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
                case 15:
                    for(int i = 0; i < page; i++)
                    {
                        if(q[proc].pages[i] == 1)
                            pageout(proc,i);
                    }
                    pagein(proc,page);
                    pagein(proc,page+1);
                    for(int i = page +2; i < 20; i++)
                    {
                        if( i != futurepage)
                        {
                            if( q[proc].pages[i] == 1 )
                            {
                                pageout( proc , i );
                            }
                        }
                    }
                    break;
            }
        }
    }

	/* TODO: Implement Predictive Paging */

	/* advance time for next pageit iteration */
	tick++;
}
