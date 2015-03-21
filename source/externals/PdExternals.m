//
//  pdExternals.m
//  NoteLet
//
//  Created by Chris Penny on 3/4/15.
//  Copyright (c) 2015 Intrinsic Audio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PdExternals.h"

//extern void setup_0x3c0x7e(void);
extern void dollarg_setup(void);
extern void sinh_tilde_setup(void);
extern void setup_0x3c0x7e(void);
extern void setup_0x3d0x3d0x7e(void);
extern void s2l_setup(void);
extern void limiter_tilde_setup(void);

@interface PdExternals ()

@end

@implementation PdExternals : NSObject


+(void)setup {
    
    dollarg_setup();
    sinh_tilde_setup();
    setup_0x3c0x7e();
    setup_0x3d0x3d0x7e();
    s2l_setup();
    limiter_tilde_setup();
    
}


@end
