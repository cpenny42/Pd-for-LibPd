//
//  pdExternals.m
//  NoteLet
//
//  Created by Chris Penny on 3/4/15.
//  Copyright (c) 2015 Intrinsic Audio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PdExternals.h"

extern void argv_setup(void);
extern void soundfonts_setup(void);
extern void tb_peakcomp_tilde_setup(void);
extern void allhammers_setup(void);
extern void allsickles_setup(void);

@interface PdExternals ()

@end

@implementation PdExternals : NSObject


+(void)setup {
    
    argv_setup();
    soundfonts_setup();
    tb_peakcomp_tilde_setup();
    allhammers_setup();
    allsickles_setup();
}


@end
