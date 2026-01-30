//
//  lab3_covTestsOC.m
//  lab3-covTestsOC
//
//  Created by Badr on 1/29/26.
//

#import <XCTest/XCTest.h>

@interface lab3_covTestsOC : XCTestCase

@end

@implementation lab3_covTestsOC

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testCatch2Suite {
    // Find the products directory (where the built executable lives)
    NSString *bundlePath = [[NSBundle bundleForClass:[self class]] bundlePath];
    NSString *productsDir = [bundlePath stringByDeletingLastPathComponent];

    // IMPORTANT: executable name. Use "lab3-cov" unless your product is named differently.
    NSString *exePath = [productsDir stringByAppendingPathComponent:@"lab3-cov"];

    NSTask *task = [[NSTask alloc] init];
    task.launchPath = exePath;
    task.arguments = @[ @"--test" ];

    NSPipe *pipe = [NSPipe pipe];
    task.standardOutput = pipe;
    task.standardError = pipe;

    [task launch];
    [task waitUntilExit];

    NSData *data = [[pipe fileHandleForReading] readDataToEndOfFile];
    NSString *output = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];

    XCTAssertEqual(task.terminationStatus, 0, @"Catch2 failed:\n%@", output);
}


- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
