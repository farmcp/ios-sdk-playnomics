//
// Created by jmistral on 10/3/12.
//
#import <Foundation/Foundation.h>
#import "PlaynomicsFrame.h"

/**
 * Protocol describing the methods an ad click action delegate should handle
 */
@protocol PNAdClickActionHandler

// Called on the delegate to perform some action when the ad is clicked by the user
- (void)performActionOnAdClicked;

@end


/**
 * Central messaging class used to generate ad frames and register click handlers
 *
 */
@interface PlaynomicsMessaging : NSObject

// The delegate all Playnomics Execution calls will be forwarded to.  Only one execution delegate can be
//   set at any given time.
@property (retain) id delegate;
@property (nonatomic)BOOL isTesting;

// Return the shared messaging instance used by all clients
+ (PlaynomicsMessaging *)sharedInstance;

// Register an action handler with the messaging framework bound to the provided label
- (void)registerActionHandler:(id<PNAdClickActionHandler>)clickAction withLabel:(NSString *)label;

// Initialize a frame using data retrieved from the Playnomics Messaging Server.  The returned instance is
// AUTORELEASED and must be retained by the clients.
- (PlaynomicsFrame *)createFrameWithId:(NSString *)frameId;


@end