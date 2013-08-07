//
// Created by jmistral on 10/3/12.
//

#import <Foundation/Foundation.h>

typedef enum {
    DisplayResultNoInternetPermission,  // Communication with the ad server is impossible
    DisplayResultStartNotCalled,  // Data collection API was not initialized
    DisplayResultUnableToConnect,  // No successful connections to the ad server have occurred
    DisplayResultFailUnknown,  // Any other problem (included bad responses from the ad server)
    DisplayResultDisplayPending,  // Ad server has been reached, but assets not ready yet, will display when ready
    DisplayAdColony,  // Show an AdColony video ad
    DisplayResultDisplayed  // Success
} DisplayResult;

// Represents the container for the ad image.
//
// This frame frame will be responsible for displaying the ad image and capturing all of
// clicks within the ad area.
@interface PlaynomicsFrame : NSObject

// Frame ID as provided by the developer when retrieving the frame
@property (retain) NSString *frameId;

// Called to display the ad frame and to begin capturing clicks within the frame.
- (DisplayResult)start;

- (void)sendVideoView;

@end