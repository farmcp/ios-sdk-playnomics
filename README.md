Playnomics PlayRM iOS SDK Integration Guide
=============================================
## Considerations for Cross-Platform Games

If you want to deploy your game to multiple platforms (eg: iOS, Android, etc), you'll need to create a separate Playnomics Applications in the control panel. Each application must incorporate a separate `<APPID>` particular to that application. In addition, message frames and their respective creative uploads will be particular to that app in order to ensure that they are sized appropriately - proportionate to your game screen size.

Getting Started
===============

## Download and Installing the SDK

You can download the SDK by forking this repo or [downloading](https://github.com/playnomics/ios-sdk/archive/master.zip) the archived files. All of the necessary install files are in the *build* folder:

* libplaynomics.a
* Playnomics.h
* PNLogger.h

Then import the SDK files into your existing game through Xcode:

<img src="http://integration.playnomics.com/img/ios/xcode-import.png"/>

## Starting a PlayRM Session

To start logging automatically tracking player engagement data, you need to first start a session. **No other SDK calls will work until you do this.**

In the class that implements `AppDelegate`, start the PlayRM Session in the `didFinishLaunchingWithOptions` method.

```objectivec
#import "AppDelegate.h"
#import "Playnomics.h"

@implementation AppDelegate

- (BOOL) application: (UIApplication *) application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //Enable test mode to view your events in the Validator. Remove this line of code before releasing your game to the app store.
    [Playnomics setTestMode: YES];
    const unsigned long long applicationId = <APPID>;
    [Playnomics startWithApplicationId:applicationId];

    //other code to initialize your iOS application below this
}
```
You can either provide a dynamic `<USER-ID>` to identify each player:

```objectivec
+ (BOOL) startWithApplicationId:(unsigned long long) applicationId userId: (NSString *) userId;
```

or have PlayRM, generate a *best-effort* unique-identifier for the player:

```objectivec
+ (BOOL) startWithApplicationId:(unsigned long long) applicationId;
```

If you do choose to provide a `<USER-ID>`, this value should be persistent, anonymized, and unique to each player. This is typically discerned dynamically when a player starts the game. Some potential implementations:

* An internal ID (such as a database auto-generated number).
* A hash of the user's email address.

**You cannot use the user's Facebook ID or any personally identifiable information (plain-text email, name, etc) for the `<USER-ID>`.**

## Tracking Intensity

To track player intensity, PlayRM needs to know about UI events occurring in the game. We provide an implementation of `UIApplication<UIApplicationDelegate>`, which automatically captures these events. In the **main.m** file of your iOS application, you pass this class name into the `UIApplicationMain` method:

```objectivec
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "Playnomics.h"

int main(int argc, char *argv[])
{
    @autoreleasepool {
        return UIApplicationMain(argc, argv, NSStringFromClass([PNApplication class]), NSStringFromClass([AppDelegate class]));
    }
}
```

If you already have your own implementation of `UIApplication<UIApplicationDelegate>` in main.m, just add the following code snippet to your class implementation:

```objectivec
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Playnomics.h"

@implementation YourApplication
- (void) sendEvent: (UIEvent *) event {
    [super sendEvent:event];
    [Playnomics onUIEventReceived:event];
}
@end
```

## Validate Integration
After you've finished the installation, you should verify your that application is correctly integrated by checkout the integration verification section of your application page.

Simply visit the self-check page for your application: **`https://controlpanel.playnomics.com/validation/<APPID>`**

The page will update with events as they occur in real-time, with any errors flagged. Visit the  <a href="http://integration.playnomics.com/technical/#self-check">self-check validation guide</a> for more information.

We strongly recommend running the self-check validator before deploying your newly integrated application to production.

## Switch SDK to Production Mode

Once you have [validated](#validate-integration) your integration, switch the SDK from **test** to **production** mode by simply setting `setTestMode` field to `NO` (or by removing/commenting out the call entirely) in the initialization block:

```objectivec
//...

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //...
    [PlaynomicsSession setTestMode:NO];
    [PlaynomicsSession startWithApplicationId:applicationId];
    //...
}
```
If you ever wish to test or troubleshoot your integration later on, simply set `setTestMode` back to `YES` and revisit the self-check validation tool for your application:

**`https://controlpanel.playnomics.com/applications/<APPID>`**

**Congratulations!** You've completed our basic integration. You will now be able to track engagement data through the PlayRM dashboard. PlayRM also enables you to message players with internal offer your players, and to track monetization and custom events.

Full Integration
================

<div class="outline">
    <ul>
        <li>
            <a href="#messaging-integration">Messaging Integration</a>
            <ul>
                <li>
                    <a href="#sdk-integration">SDK Integration</a>
                </li>
                <li>
                    <a href="#enabling-code-callbacks">Enabling Code Callbacks</a>
                </li>
            </ul>
        </li>
        <li>
            <a href="#monetization">Monetization</a>
        </li>
        <li>
            <a href="#custom-event-tracking">Custom Event Tracking</a>
        </li>
        <li>
            <a href="#push-notfications">Push Notifications</a>
        </li>
        <li>
            <a href="#support-issues">Support Issues</a>
        </li>
        <li>
            <a href="#change-log>">Change Log</a>
        </li>
    </ul>
</div>


## Monetization

PlayRM allows you to track monetization through in-app purchases denominated in real US dollars.

```objectivec
+ (void) transactionWithUSDPrice: (NSNumber *) priceInUSD
                        quantity: (NSInteger) quantity;
```
<table>
    <thead>
        <tr>
            <th>Name</th>
            <th>Type</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>priceInUSD</code></td>
            <td>NSNumber *</td>
            <td>The price of the item in USD.</td>
        </tr>
        <tr>
            <td><code>quantity</code></td>
            <td>NSInteger</td>
            <td>
               The number of items being purchased at the price specified.
            </td>
        </tr>
    </tbody>
</table>


```objectivec



```


## Custom Event Tracking

Milestones may be defined in a number of ways.  They may be defined at certain key gameplay points like, finishing a tutorial, or may they refer to other important milestones in a player's lifecycle. PlayRM, by default, supports up to five custom milestones.  Players can be segmented based on when and how many times they have achieved a particular milestone.

Each time a player reaches a milestone, track it with this call:

```objectivec
+ (PNAPIResult) milestoneWithId: (signed long long) milestoneId
                        andName: (NSString *) milestoneName;
```
<table>
    <thead>
        <tr>
            <th>Name</th>
            <th>Type</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>milestoneId</code></td>
            <td>signed long long</long>
            <td>
                A unique 64-bit numeric identifier for this milestone occurrence.
            </td>
        </tr>
        <tr>
            <td><code>andName</code></td>
            <td>NSString *</td>
            <td>
                The name of the milestone, which should be "CUSTOMn", where n is 1 through 5.
                The name is case-sensitive.
            </td>
        </tr>
    </tbody>
</table>

Example client-side calls for a player reaching a milestone, with generated IDs:

```objectivec

//when milestone CUSTOM1 is reached
int milestoneCustom1Id = arc4random();
[PlaynomicsSession milestoneWithId: milestoneCustom2Id andName: "CUSTOM1"];
```

Messaging Integration
=====================
This guide assumes you're already familiar with the concept of frames and messaging, and that you have all of the relevant `frames` setup for your application.

If you are new to PlayRM's messaging feature, please refer to <a href="http://integration.playnomics.com" target="_blank">integration documentation</a>.

Once you have all of your frames created with their associated `<PLAYRM-FRAME-ID>`s, you can start the integration process.

## SDK Integration

To work with, the messaging you'll need to import the appropriate header files into your UIViewController header file. 

```objectivec
#import <UIKit/UIKit.h>
#import "PlaynomicsSession.h"
#import "PlaynomicsFrame.h"
#import "PlaynomicsMessaging.h"

@interface ViewController : UIViewController <UITextFieldDelegate>
{
    @private
    PlaynomicsFrame frame;
}
```

Then you need to obtain a reference to the `PlaynomicsMessaging` singleton:

```objectivec
PlaynomicsMessaging *messaging = [PlaynomicsMessaging sharedInstance];
```

Loading frames through the SDK:

```objectivec
- (PlaynomicsFrame *) createFrameWithId:(NSString *)frameId;
```
<table>
    <thead>
        <tr>
            <th>Name</th>
            <th>Type</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>frameId</code></td>
            <td>NSString*</td>
            <td>Unique identifier for the frame, the <code>&lt;PLAYRM-FRAME-ID&gt;</code></td>
        </tr>
    </tbody>
</table>

Optionally, associate a class that can response to PNFrameDelegate protocol, to process rich data callbacks. See [Using Rich Data Callbacks](#using-rich-data-callbacks) for more information.

```objectivec
- (PlaynomicsFrame *)createFrameWithId:(NSString*)frameId frameDelegate: (id<PNFrameDelegate>)frameDelegate;
```
<table>
    <thead>
        <tr>
            <th>Name</th>
            <th>Type</th>
            <th>Description</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>frameId</code></td>
            <td>NSString*</td>
            <td>Unique identifier for the frame, the <code>&lt;PLAYRM-FRAME-ID&gt;</code></td>
        </tr>
        <tr>
            <td><code>frameDelegate</code></td>
            <td>id&lt;PNFrameDelegate&gt;</td>
            <td>
                Processes rich data callbacks, see <a href="#using-rich-data-callbacks">Using Rich Data Callbacks</a>
            </td>
        </tr>
    </tbody>
</table>

If you are not using ARC, keep in mind that:
* You do not need to `release` the PlaynomicsFrame object. The PlayRM SDK will automatically release the frame when it is closed.
* You do need to `release` PNFrameDelegate. PlayRM only stores a `weak` reference to the delegate to avoid strong reference cycles.

* Frames are loaded asynchronously to keep your game responsive. The `createFrameWithId` call begins the frame loading process. However, until you call `start` on the frame, the frame will not be drawn in the UI. This gives you control over when a frame will appear. Frames are destroyed on a ViewController transition or when closed.

In the example below, we initialize the frame when view is visible and then show it in another event delegate.

In practice, a frame can be loaded in a variety of ways.

```objectivec
#import "ViewController.h"

@implementation ViewController{
    PlaynomicsFrame* _frame;
}

- (void)viewDidLoad{
    PlaynomicsMessaging *messaging = [PlaynomicsMessaging sharedInstance];
    _frame = [messaging createFrameWithId: @"<PLAY-FRAME-ID>"];
}

-void someOtherEvent{
    [_frame start];
}
@end
```

## Using Rich Data Callbacks

Depending on your configuration, a variety of actions can take place when a frame's message is pressed or clicked:

* Redirect the player to a web URL in Safari
* Firing a Rich Data callback in your game
* Or in the simplest case, just close, provided that the **Close Button** has been configured correctly.

Rich Data is a JSON message that you associate with your message creative. When the player presses the message, the PlayRM SDK bubbles-up the associated JSON object to an object that can respond to the protocol, `PNFrameDelegate`, associated with the frame.

```objectiveC
@protocol PNFrameDelegate <NSObject>
@required
    -(void) onClick: (NSDictionary*) jsonData;
@end
```
The actual contents of your message can be delayed until the time of the messaging campaign configuration. However, the structure of your message needs to be decided before you can process it in your game. 

**The Rich Data callback will not fire if the Close button is pressed.**

Here are three common use cases for frames and a messaging campaigns

* [Game Start Frame](#game-start-frame)
* [Event Driven Frame - Open the Store](#event-driven-frame-open-the-store) for instance, when the player is running low on premium currency
* [Event Driven Frame - Level Completion](#event-driven-drame-level-completion)


### Game Start Frame

In this use-case, we want to configure a frame that is always shown to players when they start playing a new game. The message shown to the player may change based on the desired segments:

<table>
    <thead>
        <tr>
            <th>
                Segment
            </th>
            <th>
                Priority
            </th>
            <th>
                Callback Behavior
            </th>
            <th>
                Creative
            </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>
                At-Risk
            </td>
            <td>1st</td>
            <td>
                In this case, we're worried once-active players are now in danger of leaving the game. We might offer them <strong>50 MonsterBucks</strong> to bring them back.
            </td>
            <td>
                <img src="http://playnomics.com/integration-dev/img/messaging/50-free-monster-bucks.png"/>
            </td>
        </tr>
        <tr>
            <td>
                Lapsed 7 or more days
            </td>
            <td>2nd</td>
            <td>
                In this case, we want to thank the player for coming back and incentivize these lapsed players to continue doing so. We might offer them <strong>10 MonsterBucks</strong> to increase their engagement and loyalty.
            </td>
            <td> 
                <img src="http://playnomics.com/integration-dev/img/messaging/10-free-monster-bucks.png"/>
            </td>
        </tr>
        <tr>
            <td>
                Default - players who don't fall into either segment.
            </td>
            <td>3rd</td>
            <td>
                In this case, we can offer a special item to them for returning to the grame.
            </td>
            <td>
                <img src="http://playnomics.com/integration-dev/img/messaging/free-bfb.png"/>
            </td>
        </tr>
    </tbody>
</table>

```objectivec
//AwardFrameDelegate.h

#import <Foundation/Foundation.h>
#import "PlaynomicsMessaging.h"
@interface AwardFrameDelegate : NSObject<PNFrameDelegate>
@end

//AwardFrameDelegate.m

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AwardFrameDelegate.h"
#import "Inventory.h"

@implementation AwardFrameDelegate
- (void)onClick:(NSDictionary *)jsonData{
    if([data objectForKey: @"type"] != (id)[NSNull null] &&
         [[data objectForKey:@"type"] isEqualToString: @"award"]){
        
        if([data objectForKey: @"award"] != (id)[NSNull null]){
            NSDictionary* award = [data objectForKey: @"award"];
            
            NSString* item = [award objectForKey: @"item"];
            NSNumber* quantity = [award objectForKey: @"quantity"];

            //call your own inventory object
            [[Inventory sharedInstanced] addItem:item andQuantity: quanity];
        }
    }
}
@end
```

And then attaching this AwardFrameDelegate class to the frame shown in the first game scene:

```objectiveC
@implementation GameViewController{
    AwardFrameDelegate* _awardDelegate;
    PlaynomicsFrame* _frame;
}
-(void) viewDidLoad{
    PlaynomicsMessaging* messaging = [PlaynomicsMessaging sharedInstance];
    _awardDelegate = [[AwardFrameDelegate alloc] init];
    _frame = [messaging createFrameWithId : frameId frameDelegate : _awardDelegate];
    [_frame start];
}

-(void) dealloc{
    //make sure to release the delegate, if you are not using ARC
    [_awardDelegate release];
    [super dealloc];
}
@end
```

The related messages would be configured in the Control Panel to use this callback by placing this in the **Target Data** for each message:

Grant 10 Monster Bucks
```json
{
    "type" : "award",
    "award" : 
    {
        "item" : "MonsterBucks",
        "quantity" : 10
    }
}
```

Grant 50 Monster Bucks
```json
{
    "type" : "award",
    "award" : 
    {
        "item" : "MonsterBucks",
        "quantity" : 50
    }
}
```

Grant Bazooka
```json
{
    "type" : "award",
    "award" :
    {
        "item" : "Bazooka",
        "quantity" : 1
    }
}
```

### Event Driven Frame - Open the Store

An advantage of a *dynamic* frames is that they can be triggered by in-game events. For each in-game event you would configure a separate frame. While segmentation may be helpful in deciding what message you show, it may be sufficient to show the same message to all players.

In particular one event, for examle, a player may deplete their premium currency and you want to remind them that they can re-up through your store. In this context, we display the same message to all players.

<table>
    <thead>
        <tr>
            <th>
                Segment
            </th>
            <th>
                Priority
            </th>
            <th>
                Callback Behavior
            </th>
            <th>
                Creative
            </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>
                Default - all players, because this message is intended for anyone playing the game.
            </td>
            <td>1st</td>
            <td>
                You notice that the player's in-game, premium currency drops below a certain threshold, now you can prompt them to re-up with this <strong>message</strong>.
            </td>
            <td>
                <img src="http://playnomics.com/integration-dev/img/messaging/running-out-of-monster-bucks.png"/>
            </td>
        </tr>
    </tbody>
</table>

```objectivec
//StoreFrameDelegate.h

#import <Foundation/Foundation.h>
#import "PlaynomicsMessaging.h"
@interface StoreFrameDelegate : NSObject<PNFrameDelegate>
@end

//StoreFrameDelegate.m

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "StoreFrameDelegate.h"
#import "Inventory.h"

@implementation StoreFrameDelegate
- (void)onClick:(NSDictionary *)jsonData{
    if([data objectForKey: @"type"] != (id)[NSNull null] && 
        [[data objectForKey:@"type"] isEqualToString: @"action"]){
        
        if([data objectForKey: @"action"] != (id)[NSNull null] && 
            [[data objectForKey:@"type"] isEqualToString: @"openStore"]){
            
            [[Store sharedInstance] open];
        
        }
    }
}
@end
```

The Default message would be configured in the Control Panel to use this callback by placing this in the **Target Data** for the message :

```json
{
    "type" : "action",
    "action" : "openStore"
}
```

### Event Driven Frame - Level Completion

In the following example, we wish to generate third-party revenue from players unlikely to monetize by showing them a segmented message after completing a level or challenge: 

<table>
    <thead>
        <tr>
            <th>
                Segment
            </th>
            <th>
                Priority
            </th>
            <th>
                Callback Behavior
            </th>
            <th>
                Creative
            </th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>
                Non-monetizers, in their 5th day of game play
            </td>
            <td>1st</td>
            <td>Show them a 3rd party ad, because they are unlikely to monetize.</td>
            <td>
                <img src="http://playnomics.com/integration-dev/img/messaging/third-party-ad.png"/>
            </td>
        </tr>
        <tr>
            <td>
                Default: everyone else
            </td>
            <td>2nd</td>
            <td>
                You simply congratulate them on completing the level and grant them some attention currency, "Mana" for completeing the level.
            </td>
            <td>
                <img src="http://playnomics.com/integration-dev/img/messaging/darn-good-job.png"/>
            </td>
        </tr>
    </tbody>
</table>

This another continuation on the `AwardFrameDelegate`, with some different data. The related messages would be configured in the Control Panel:

* **Non-monetizers, in their 5th day of game play**, a Target URL: `HTTP URL for Third Party Ad`
* **Default**, Target Data:

```json
{
    "type" : "award",
    "award" :
    {
        "item" : "Mana",
        "quantity" : 20
    }
}
```
Push Notifications
==================

## Registering for PlayRM Push Messaging

To get started with PlayRM Push Messaging, your app will need to register with Apple to receive push notifications. Do this by calling the `registerForRemoteNotificationTypes` method on UIApplication.

```objectivec
@implementation AppDelegate

//...

- (BOOL)application:(UIApplication *)application 
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    const long long applicationId = <APPID>;
    [PlaynomicsSession startWithApplicationId:applicationId];

    //enable notifications
    UIApplication *app = [UIApplication sharedApplication];
    [app registerForRemoteNotificationTypes: (UIRemoteNotificationTypeBadge 
        | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
    //...
}
```

Once the player, authorizes push notifications from your app, you need to provide Playnomics with player's device token

```objectivec
@implementation AppDelegate

//...

-(void)application:(UIApplication *)application 
    didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
   [PlaynomicsSession enablePushNotificationsWithToken:deviceToken];
}
```

## Push Messaging Impression and Click Tracking

There are 3 situations in which an iOS device can receive a Push Notification

<table>
    <thead>
        <tr>
            <th>Sitatuation</th>
            <th>Push Message Shown?</th>
            <th>Delegate Handler</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>App is not running</td>
            <td rowspan="2">Yes</td>
            <td>didFinishLaunchingWithOptions:(NSDictionary*)launchOptions</td>
        </tr>
        <tr>
            <td>App is running in the background</td>
            <td rowspan="2">
                didReceiveRemoteNotification:(NSDictionary*)userInfo
            </td>
        </tr>
        <tr>
            <td>App is running in the foreground</td>
            <td>No</td>
        </tr>
    </tbody>
</table>

The first situation is automatically handled by the Playnomics SDK. The other two situations, however, need to be implemented in the `didReceiveRemoteNotification` method:

```objectivec
-(void) application:(UIApplication *)application
    didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    NSMutableDictionary *payload = [userInfo mutableCopy];
    [PlaynomicsSession pushNotificationsWithPayload:payload];
    [payload release];
}
```

By default, iOS does not show push notifications when your app is already in the foreground. Consequently, PlayRM does NOT track these push notifications as impressions nor clicks. However, if you do circumvent this default behavior and show the Push Notification when the app is in the foreground, you can override this functionality by adding the following line of code in the `didReceiveRemoteNotification` method:

```objectivec
    [payload setObject:[NSNumber numberWithBool:NO] forKey:@"pushIgnored"];
```

This will allow each push notification to be treated as a click even if the app is in the foreground.

## Clearing Push Badge Numbers

When you send push notifications, you can configure a badge number that will be set on your application icon in the home screen. When you send push notifications, you can configure a badge number that will be set on your application. iOS defers the responsibility of resetting the badge number to the developer. 

To do this, insert this code snippet in the `applicationWillResignActive` method of your `UIAppDelegate`

```objectivec
- (void)applicationWillResignActive:(UIApplication *)application {
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}
```

Support Issues
==============
If you have any questions or issues, please contact <a href="mailto:support@playnomics.com">support@playnomics.com</a>.

Change Log
==========
#### Version 9
* Adding support for Rich Data Callbacks
* Targeting the arm7, arm7s, i386 CPU Arcitectures
* Now compatible with iOS 5 and above
* Supporting touch events for Cocos2DX

####  Version 8.2
* Support for video ads
* Capture advertising tracking information

####  Version 8.1.1
* Renamed method in PlaynomicsMessaging.h from "initFrameWithId" to "createFrameWithId"
* Minor bug fixes

####  Version 8.1
* Support for push notifications
* Minor bug fixes

####  Version 8
* Support for internal messaging
* Added milestone module

####  Version 7
* Support for new iOS hardware, iPhone 5s

#### Version 6
* Improved dual support for iOS4 and iOS5+ utilizing best methods depending on runtime version
* This build is a courtesy build provided for debugging for an unreproducible customer-reported crash that was unrelated to PlayRM code. 

#### Version 4
support for iOS version 4.x

####  Version 3
* Improved crash protection
* Ability to run integrated app on the iOS simulator
* Minor tweaks to improve connection to server

#### Version 2
* First production release

View version tags <a href="https://github.com/playnomics/ios-sdk/tags">here</a>
