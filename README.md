Playnomics PlayRM iOS SDK Integration Guide
=============================================
If you're new to PlayRM and/or don't have a PlayRM account and would like to get started using PlayRM please visit   <a href="https://controlpanel.playnomics.com/signup">https://controlpanel.playnomics.com/signup</a>   to sign up. Soon after creating an account you will receive a registration confirmation email permitting you access to your PlayRM control panel.

Within the control panel, click the <strong>applications</strong> tab and add your game. Upon doing so, you will recieve an <strong>Application ID</strong> and an <strong>API KEY</strong>. These two components will enable you to begin the integration process.

Our integration has been optimized to be as straight forward and user friendly as possible. If you're feeling unsure or would like better understand the order the process before beginning integration, please take a moment to check out the <a href="http://integration.playnomics.com/technical/#integration-getting-started">getting started</a> page. Here you can find an overview of our integration process, and platform specific features, to help you better understand the PlayRM integration process.

Note, this is SDK is intended for working with native iOS games built with Xcode, if you're using Unity and deploying your game to iOS, please refer to the <a target="_blank" href="https://github.com/playnomics/unity-sdk#playnomics-playrm-unity-sdk-integration-guide">PlayRM Unity SDK</a>.


## Considerations for Cross-Platform Games

If you want to deploy your game to multiple platforms (eg: iOS and the Unity Web player), you'll need to create a separate Playnomics Applications in the control panel. Each application must incorporate a separate `<APPID>` particular to that application. In addition, message frames and their respective creative uploads will be particular to that app in order to ensure that they are sized appropriately - proportionate to your game screen size.




Basic Integration
=================

You can download the SDK by forking this repo or downloading the archived files. All of the necessary install files are in the *build* folder:

* libplaynomics.a
* PlaynomicsFrame.h
* PlaynomicsMessaging.h
* PlaynomicsSession.h

Then import the SDK files into your existing game through Xcode:

<img src="http://integration.playnomics.com/img/ios/xcode-import.png"/>

### Interacting with PlayRM in Your Game

All session-related calls are made through a `SharedInstance` class `PlaynomicsSession` while all messaging-related calls are made through a `SharedInstance` class `PlaynomicsMessaging`. To work with any of these classes, you need to import the appropriate header file:

```objective-c
#import PlaynomicsSession.h
```

All public methods, except for messaging specific calls, return an enumeration `PNAPIResult`. The values for this enumeration are:

<table>
    <thead>
        <tr>
            <td>Enumeration</td>
            <td>Description</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><code>PNAPIResultSent</code></td>
            <td>
                The event has been sent to PlayRM.
            </td>
        </tr>        
        <tr>
            <td><code>PNAPIResultAlreadyStarted</code></td>
            <td>
                You've already started the session. [PlaynomicsSession startWithApplicationId]</code> was called unnecessarily.
            </td>
        </tr>
        <tr>
            <td><code>PNAPIResultStartNotCalled</code></td>
            <td>
                You didn't start the session. The SDK won't be able to report any data to the PlayRM RESTful API, until this has been done.
            </td>
        </tr>
        <tr>
            <td><code>PNAPIResultFailUnkown</code></td>
            <td>
                An unknown exception occurred.
            </td>
        </tr>
    </tbody>
</table>

**You always need to start a session before making any other SDK calls.**

### Starting a Player Session

To start collecting behavior data, you need to initialize the PlayRM session. In the class that implements `AppDelegate`, start the PlayRM Session in the `didFinishLaunchingWithOptions` method.

You can either provide a dynamic `<USER-ID>` to identify each player:

```objectivec
+ (PNAPIResult) startWithApplicationId:(signed long long) applicationId
                                userId: (NSString *) userId;
```

or have PlayRM, generate a *best-effort* unique-identifier for the player:

```objectivec
+ (PNAPIResult) startWithApplicationId:(signed long long) applicationId;
```

If you do choose to provide a `<USER-ID>`, this value should be persistent, anonymized, and unique to each player. This is typically discerned dynamically when a player starts the game. Some potential implementations:

* An internal ID (such as a database auto-generated number).
* A hash of the user's email address.

**You cannot use the user's Facebook ID or any personally identifiable information (plain-text email, name, etc) for the `<USER-ID>`.**

You **MUST** make the initialization call before working with any other PlayRM modules. You only need to call this method once.

```objectivec
#import "AppDelegate.h"
#import "PlaynomicsSession.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    const long long applicationId = <APPID>;

    [PlaynomicsSession setTestMode:YES];
    [PlaynomicsSession startWithApplicationId:applicationId];
    //other code to initialize the application below this
}
```


Congratulations! You've completed our basic integration. You will now be able to track engagement behaviors (having incorporated the Engagement Module) from the PlayRM dashboard. At this point we recomend that you use our integration validation tool to test your integration of our SDK in order insure that it has been properly incorporated in your game. 


PlayRM is currently operating in test mode. Be sure you switch to [production mode](#switch-sdk-to-production-mode), by implementing the code call outlined in our Basic Integration before deploying your game on the web or in an app store.


# Full Integration


<div class="outline">
<li>
<a href="#full-integration">Full Integration</a>
<ul>
<li><a href="#demographics-and-install-attribution">Demographics and Install Attribution</a></li>
<li>
<a href="#monetization">Monetization</a>
<ul>
<li><a href="#purchases-of-in-game-currency-with-real-currency">Purchases of In-Game Currency with Real Currency</a></li>
<li><a href="#purchases-of-items-with-real-currency">Purchases of Items with Real Currency</a></li>
<li><a href="#purchases-of-items-with-premium-currency">Purchases of Items with Premium Currency</a></li>
</ul>
</li>
<li><a href="#invitations-and-virality">Invitations and Virality</a></li>
<li><a href="#custom-event-tracking">Custom Event Tracking</a></li>
<li><a href="#validate-integration">Validate Integration</a></li>
<li><a href="#switch-sdk-to-production-mode">Switch SDK to Production Mode</a></li>
</ul>
</li>
<li>
<a href="#messaging-integration">Messaging Integration</a>
<ul>
<li><a href="#sdk-integration">SDK Integration</a></li>
<li><a href="#enabling-code-callbacks">Enabling Code Callbacks</a></li>
</ul>
</li>
<ul>
<li><a href="#push-notfications">Push Notifications</a></li>
</ul>
<ul>
<li><a href="#support-issues">Support Issues</a></li>
<li><a href="#change-log>"> Change Log</a></li>
</ul>
</div>


If you're reading this it's likely that you've integrated our SDK and are interested in tailoring PlayRM to suit your particular segmentation needs.

The index on the right provides a holistic overview of the <strong>full integration</strong> process. From it, you can jump to specific points in this document depending on what you're looking to learn and do.

To clarify where you are in the timeline of our integration process, you've completed our basic integration. Doing so will enable you to track engagement behaviors from the PlayRM dashboard (having incorporated the Engagement Module). The following documentation will provides succint information on how to incorporate additional and more in-depth segmentation functionality by integrating any, or all of the following into your game:


<ul>
<li><strong>User Info Module:</strong> - provides basic user information</li>
<li><strong>Monetization Module:</strong> - tracks various monetization events and transactions</li>
<li><strong>Virality Module:</strong> - tracks the social activities of users</li>
<li><strong>Milestone Module:</strong> - tracks significant player events customized to your game</li>
</ul>


Along with integration instructions for our various modules, you will also find integration information pertaining to messaging frame setup, and push setup within this documentation.

## Demographics and Install Attribution

After the SDK is loaded, the user info module may be called to collect basic demographic and acquisition information. This data is used to segment users based on how/where they were acquired and enables improved targeting with basic demographics, in addition to the behavioral data collected using other events.

Provide each user's information using this call:

```objectivec
+ (PNAPIResult) userInfoForType: (PNUserInfoType) type 
                        country: (NSString *) country 
                    subdivision: (NSString *) subdivision
                            sex: (PNUserInfoSex) sex
                       birthday: (NSDate *) birthday
                         source: (PNUserInfoSource) source 
                 sourceCampaign: (NSString *) sourceCampaign 
                    installTime: (NSDate *) installTime;
```

If any of the parameters are not available, you should pass `nil`.

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
            <td>
                <code>type</code>
            </td>
            <td>
                PNUserInfoType
            </td>
            <td>
                There is currently only one option available for this: <strong>PNUserInfoTypeUpdate</strong> for userInfo updates.
            </td>
        </tr>
        <tr>
            <td><code>country</code></td>
            <td>NSString *</td>
            <td>This has been deprecated. Just pass <code>nil</code>.</td>
        </tr>
        <tr>
            <td><code>subdivision</code></td>
            <td>NSString *</td>
            <td>This has been deprecated. Just pass <code>nil</code>.</td>
        </tr>
        <tr>
            <td><code>sex</code></td>
            <td>PNUserInfoSex</td>
            <td>
                <ul>
                    <li>PNUserInfoSexMale</li>
                    <li>PNUserInfoSexFemale</li>
                    <li>PNUserInfoSexUnknown</li>
                </ul>
            </td>
        </tr>
        <tr>
            <td><code>birthday</code></td>
            <td>NSDate</td>
            <td>A birthday for the player.</td>
        </tr>
        <tr>
            <td><code>sourceAsString</code></td>
            <td>NSString *</td>
            <td>
                Source of the user, such as "FacebookAds", "UserReferral", "Playnomics", etc. These are only suggestions, any 16-character or shorter string is acceptable.
            </td>
        </tr>
        <tr>
            <td><code>sourceCampaign</code></td>
            <td>NSString *</td>
            <td>
                Any 16-character or shorter string to help identify specific campaigns
            </td>
        </tr>
        <tr>
            <td><code>installTime</code></td>
            <td>NSDate *</td>
            <td>
                When the player originally installed the game.
            </td>
        </tr>
    </tbody>
</table>

This method is overloaded, with the ability to use an enum for the source

```objectivec
+ (PNAPIResult) userInfoForType: (PNUserInfoType) type 
                        country: (NSString *) country 
                    subdivision: (NSString *) subdivision
                            sex: (PNUserInfoSex) sex
                       birthday: (NSDate *) birthday
                 sourceAsString: (NSString *) source 
                 sourceCampaign: (NSString *) sourceCampaign 
                    installTime: (NSDate *) installTime;
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
            <td><code>source</code></td>
            <td>PNUserInfoSource</td>
            <td>
                You can use a predefined enumeration of PNUserInfoSource
                <br/>
                <ul>
                    <li>PNUserInfoSourceAdwords</li>
                    <li>PNUserInfoSourceDoubleClick</li>
                    <li>PNUserInfoSourceYahooAds</li>
                    <li>PNUserInfoSourceMSNAds</li>
                    <li>PNUserInfoSourceAOLAds</li>
                    <li>PNUserInfoSourceAdbrite</li>
                    <li>PNUserInfoSourceFacebookAds</li>
                    <li>PNUserInfoSourceGoogleSearch</li>
                    <li>PNUserInfoSourceYahooSearch</li>
                    <li>PNUserInfoSourceBingSearch</li>
                    <li>PNUserInfoSourceFacebookSearch</li>
                    <li>PNUserInfoSourceApplifier</li>
                    <li>PNUserInfoSourceAppStrip</li>
                    <li>PNUserInfoSourceVIPGamesNetwork</li>
                    <li>PNUserInfoSourceUserReferral</li>
                    <li>PNUserInfoSourceInterGame</li>
                    <li>PNUserInfoSourceOther</li>
                </ul>
            </td>
        </tr>
    </tbody>
</table>

Since PlayRM uses the game client's IP address to determine geographic location, country and subdivision should be set to `nil`.

```objectivec
#import "AppDelegate.h"
#import "PlaynomicsSession.h"
@implementation AppDelegate

//...
//...
//...

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    
    //after you initialize the PlayRM Sessions
    NSDate *installDate = nil;
    if(isNewUser){
        installDate = [NSDate date];
    }

    PNAPIResult result = [PlaynomicsSession 
                    userInfoForType: PNUserInfoTypeUpdate
                    country: nil
                    subdivision: nil
                    sex: PNUserInfoSexFemale
                    birthday: [[NSDate alloc] initWithString:@"1980-01-01"];
                    source: @"AppStore"
                    sourceCampaign: @"Facebook Ad"
                    installTime: installDate];
}
```

## Monetization

PlayRM provides a flexible interface for tracking monetization events. This module should be called every time a player triggers a monetization event. 

This event tracks users that have monetized and the amount they have spent in total, *real* currency:
* FBC (Facebook Credits)
* USD (US Dollars)

or an in-game, *virtual* currency.

This outlines how currencies are described in PlayRM iOS

<table>
    <thead>
        <tr>
            <th>Currency</th>
            <th>Currency Data Type</th>
            <th>PNCurrencyCategoryType</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Real</td>
            <td>
                <strong>PNCurrencyType</strong>
                <ul>
                    <ul>
                        <li>PNCurrencyUSD: USD dollars</li>
                        <li>PNCurrencyFBC: Facebook Credits</li>
                    </ul>
                </ul>
            </td>
            <td>
                The <code>currencyCategory</code> is <strong>PNCurrencyCategoryReal</strong>
            </td>
        </tr>
        <tr>
            <td>Virutal</td>
            <td>
                An <strong>NSString *</strong> name, 16 characters or less.
            </td>
            <td>
                The <code>currencyCategory</code> is <strong>PNCurrencyCategoryVirtual</strong>
            </td>
        </tr>
    </tbody>
</table>


```objectivec
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
                    currencyTypes: (NSArray *) currencyTypes
                   currencyValues: (NSArray *) currencyValues
               currencyCategories: (NSArray *) currencyCategories; 
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
            <td><code>transactionId</code></td>
            <td>signed long long</td>
            <td>
                A unique identifier for this transaction. If you don't have a transaction ID from payments system, you can genenate large random number.
            </td>
        </tr>
        <tr>
            <td><code>itemId</code></td>
            <td>NSString *</td>
            <td>If applicable, an identifier for the item. The identifier should be consistent.</td>
        </tr>
        <tr>
            <td><code>quantity</code></td>
            <td>double</td>
            <td>If applicable, the number of items being purchased.</td>
        </tr>
        <tr>
            <td><code>transactionType<code></td>
            <td>PNTransactionType</td>
            <td>
                The type of transaction occurring:
                <ul>
                    <li>
                        <strong>PNTransactionBuyItem</strong>: A purchase of virtual item. The <code>quantity</code> is added to the player's inventory</li>
                    <li>
                        <strong>PNTransactionSellItem</strong>: A sale of a virtual item to another player. The item is removed from the player's inventory. Note: a sale of an item will result in two events with the same <code>transactionId</code>, one for the sale with type SellItem, and one for the receipt of that sale, with type BuyItem.
                    </li>
                    <li>
                        <strong>PNTransactionReturnItem</strong>: A return of a virtual item to the store. The item is removed from the player's inventory
                    </li>
                    <li><strong>PNTransactionBuyService</strong>: A purchase of a service, e.g., VIP membership </li>
                    <li><strong>PNTransactionSellService</strong>: The sale of a service to another player</li>
                    <li><strong>PNTransactionReturnService</strong>: The return of a service</li>
                    <li>
                        <strong>PNTransactionCurrencyConvert</strong>: A conversion of currency from one form to another, usually in the form of real currency (e.g., US dollars) to virtual currency.  If the type of a transaction is CurrencyConvert, then there should be at least 2 elements in the <code>currencyTypes</code>, 
                        <code>currencyValues</code>, and <code>currencyCategories</code>  arrays
                    </li>
                    <li>
                        <strong>PNTransactionInitial</strong>: An initial allocation of currency and/or virtual items to a new player
                    </li>
                    <li><strong>PNTransactionFree</strong>: Free currency or item given to a player by the application</li>
                    <li>
                        <strong>PNTransactionReward</strong>: Currency or virtual item given by the application as a reward for some action by the player
                    </li>
                    <li>
                        <strong>PNTransactionGiftSend</strong>: A virtual item sent from one player to another. Note: a virtual gift should result in two transaction events with the same <code>transactionId</code>, one with the type GiftSend, and another with the type GiftReceive
                    </li>
                    <li>
                        <strong>PNTransactionGiftReceive</strong>: A virtual good received by a player. See note for <strong>PNTransactionGiftSend</strong> type
                     </li>
                </ul>
            </td>
        </tr>
        <tr>
            <td><code>otherUserId</code></td>
            <td>NSString *</td>
            <td>
               If applicable, the other user involved in the transaction. A contextual example is a player sending a gift to another player.
            </td>
        </tr>
        <tr>
            <td><code>currencyTypes</code></td>
            <td>NSArray *</td>
            <td>
                An array of currencyTypes, type is either NSString * or PNCurrencyType.
            </td>
        </tr>
        <tr>
            <td><code>currencyValues</code></td>
            <td>>NSArray *</td>
            <td>An array of currency values, type is a double.</td>
        </tr>
        <tr>
            <td><code>currencyCategories</code></td>
            <td>>NSArray *</td>
            <td>
                An array of currency categories, type is PNCurrencyCategoryType.
            </td>
        </tr>
    </tbody>
</table>


An overload for `transactionWithId` with a single `currencyType` of <strong>PNCurrencyType</strong>.

```objectivec
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
                     currencyType: (PNCurrencyType) currencyType
                    currencyValue: (double) currencyValue
                 currencyCategory: (PNCurrencyCategory) currencyCategory;
```

All arguments are the same as the previous method, except that the `currencyType`,`currencyValue`, and `currencyCategory` are singular. The `currencyCategory` is likely PNCurrencyCategoryReal.


An overload for `transactionWithId` with a single `currencyTypeAsString` of <strong>NSString *</strong>.

```objectivec
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
             currencyTypeAsString: (NSString *) currencyType
                    currencyValue: (double) currencyValue
                 currencyCategory: (PNCurrencyCategory) currencyCategory;
```

All arguments are the same as the first method, except that the `currencyTypeAsString`,`currencyValue`, and `currencyCategory` are singular. The `currencyCategory` is likely PNCurrencyCategoryVirtual.

We highlight three common use-cases below.
* [Purchases of In-Game Currency with Real Currency](#purchases-of-in-game-currency-with-real-currency)
* [Purchases of Items with Real Currency](#purchases-of-items-with-real-currency)
* [Purchases of Items with In-Game Currency](#purchases-of-items-with-in-game-currency)

### Purchases of In-Game Currency with Real Currency

A very common monetization strategy is to incentivize players to purchase premium, in-game currency with real currency. PlayRM treats this like a currency exchange. This is one of the few cases where multiple currencies are used in a transaction (first implementation of `transactionWithId`. `itemId`, `quantity`, and `otherUserId` are left `null`.

```objectivec
//player purchases 500 MonsterBucks for 10 USD

NSMutableArray *currencyTypes = [NSMutableArray array];
NSMutableArray *currencyValues = [NSMutableArray array];
NSMutableArray *currencyCategories = [NSMutableArray array];

//notice that we're adding all data about each currency in order

//in-game currency
[currencyTypes addObject: @"MonsterBucks"];
[currencyValues addObject: [NSNumber numberWithDouble: 500]];
[currencyCategories addObject: [NSNumber numberWithInt: PNCurrencyCategoryVirtual]];

//real currency
[currencyTypes addObject: [NSNumber numberWithInt: PNCurrencyUSD]];
[currencyValues addObject: [NSNumber numberWithDouble: -10]];
[currencyCategories addObject: [NSNumber numberWithInt: PNCurrencyCategoryReal]];

PNAPIResult result = [PlaynomicsSession transactionWithId: transactionId 
                            itemId: nil 
                            quantity: 0
                            type: PNTransactionCurrencyConvert
                            otherUserId: nil
                            currencyTypes: currencyTypes
                            currencyValues: currencyValues
                            currencyCategories: currencyCategories];
```

### Purchases of Items with Real Currency

```objectivec
//player purchases a "Monster Trap" for $.99 USD

NSString *trapItemId = @"Monster Trap";
double quantity = 1;
double price = .99;


PNAPIResult result = [PlaynomicsSession transactionWithId: transactionId 
                            itemId: trapItemId 
                            quantity: quantity
                            type: PNTransactionBuyItem
                            otherUserId: PNCurrencyUSD
                            currencyType: currencyTypes
                            currencyValue: price
                            currencyCategory: PNCurrencyCategoryReal];
```

### Purchases of Items with Premium Currency

This event is used to segment monetized players (and potential future monetizers) by collecting information about how and when they spend their premium currency (an in-game currency that is primarily acquired using a *real* currency). This is one level of information deeper than the previous use-cases.

#### Currency Exchanges

This is a continuation on the first currency exchange example. It showcases how to track each purchase of in-game *attention* currency (non-premium virtual currency) paid for with a *premium*:

```objectivec

//In this hypothetical, Energy is an attention currency that is earned over the lifetime of the game. 
//They can also be purchased with the premium MonsterBucks that the player may have purchased earlier.

//player buys 100 Mana with 10 MonsterBucks

NSMutableArray *currencyTypes = [NSMutableArray array];
NSMutableArray *currencyValues = [NSMutableArray array];
NSMutableArray *currencyCategories = [NSMutableArray array];

//notice that we're adding all data about each currency in order
//and that both currencies are virtual

//attention currency data
[currencyTypes addObject: @"Mana"];
[currencyValues addObject: [NSNumber numberWithDouble: 100]];
[currencyCategories addObject: [NSNumber numberWithInt: PNCurrencyCategoryVirtual]];

//premium currency data
[currencyTypes addObject: @"MonsterBucks"];
[currencyValues addObject: [NSNumber numberWithDouble: -10]];
[currencyCategories addObject: [NSNumber numberWithInt: PNCurrencyCategoryVirtual]];

PNAPIResult result = [PlaynomicsSession transactionWithId: transactionId 
                            itemId: nil 
                            quantity: 0
                            type: PNTransactionCurrencyConvert
                            otherUserId: nil
                            currencyTypes: currencyTypes
                            currencyValues: currencyValues
                            currencyCategories: currencyCategories];

```
#### Item Purchases

This is a continuation on the first item purchase example, except with premium currency.

```objectivec
//player buys 20 light armor, for 5 MonsterBucks

double itemQuantity = 20;
NSSString *itemId = @"Light Armor";

NSString *premimumCurrency = @"MonsterBucks";
double premiumCost = 5;

[PNAPIResult result = transactionWithId: transactionId
                           itemId: itemId
                         quantity: itemQuantity
                             type: PNTransactionBuyItem
                      otherUserId: nil
             currencyTypeAsString: premimumCurrency
                    currencyValue: premiumCost
                 currencyCategory: PNCurrencyCategoryVirtual];
```

## Invitations and Virality

The virality module allows you to track a single invitation from one player to another (e.g., inviting friends to join a game).

If multiple requests can be sent at the same time, a separate call should be made for each recipient.

```objectivec
+ (PNAPIResult) invitationSentWithId: (signed long long) invitationId
                     recipientUserId: (NSString *) recipientUserId 
                    recipientAddress: (NSString *) recipientAddress 
                              method: (NSString *) method;
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
            <td><code>invitationId</code></td>
            <td>signed long long</td>
            <td>
                A unique 64-bit integer identifier for this invitation. 

                If no identifier is available, this could be a hash/MD5/SHA1 of the sender's and neighbor's IDs concatenated. <strong>The resulting identifier can not be personally identifiable.</strong>
            </td>
        </tr>
        <tr>
            <td><code>recipientUserId</code></td>
            <td>NSString *</td>
            <td>This can be a hash/MD5/SHA1 of the recipient's Facebook ID, their Facebook 3rd Party ID or an internal ID. It cannot be a personally identifiable ID.</td>
        </tr>
        <tr>
            <td><code>recipientAddress</code></td>
            <td>NSString *</td>
            <td>
                An optional way to identify the recipient, for example the <strong>hashed email address</strong>. When using <code>recipientUserId</code> this can be <code>null</code>.
            </td>
        </tr>
        <tr>
            <td><code>method</code></td>
            <td>NSString *</td>
            <td>
                The method of the invitation request will include one of the following:
                <ul>
                    <li>facebookRequest</li>
                    <li>email</li>
                    <li>twitter</li>
                </ul>
            </td>
        </tr>
    </tbody>
</table>

You can then track each invitation response. IMPORTANT: you will need to pass the invitationId through the invitation link.

```objectivec
+ (PNAPIResult) invitationResponseWithId: (signed long long) invitationId
                         recipientUserId: (NSString *) recipientUserId
                            responseType: (PNResponseType) responseType;
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
            <td><code>invitationId</code></td>
            <td>singed long long</td>
            <td>The ID of the corresponding <code>invitationSent</code> event.</td>
        </tr>
        <tr>
            <td><code>recipientUserId</code></td>
            <td>NSString *</td>
            <td>The <code>recipientUserID</code> used in the corresponding <code>invitationSent</code> event.</td>
        </tr>
        <tr>
            <td><code>responseType</code></td>
            <td>PNResponseType</td>
            <td>
                Currently the only response PlayRM tracks acceptance
                <ul>
                    <li>PNResponseTypeAccepted</li>
                </ul>
            </td>
        </tr>
    </tbody>
</table>

Example calls for a player's invitation and the recipient's acceptance:

```objective
int invitationId = 112345675;
NSString* recipientUserId = @"10000013";

PNAPIResult sentResult = [PlaynomicsSession invitationSentWithId: invitationId
                                    recipientUserId: recipientUserId
                                    recipientAddress: nil 
                                    method: nil];

//later on the recipient accepts the invitation

PNAPIResult responseResult = [PlaynomicsSession invitationResponseWithId: invitationId
                                    recipientUserId: recipientUserId
                                    responseType: PNResponseTypeAccepted];
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
## Validate Integration
After configuring your selected PlayRM modules, you should verify your application's correct integration with the self-check validation service.

Simply visit the self-check page for your application: **`https://controlpanel.playnomics.com/validation/<APPID>`**

You can now see the most recent event data sent by the SDK, with any errors flagged. Visit the  <a href="http://integration.playnomics.com/technical/#self-check">self-check validation guide</a> for more information.

We strongly recommend running the self-check validator before deploying your newly integrated application to production.

## Switch SDK to Production Mode
Once you have [validated](#validate-integration) your integration, switch the SDK from **test** to **production** mode by simply setting the `PlaynomicsSession`'s `setTestMode` field to `NO` (or by removing/commenting out the call entirely) in the initialization block:


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

**`https://controlpanel.playnomics.com/validation/<APPID>`**



Messaging Integration
=====================
This guide assumes you're already familiar with the concept of frames and messaging, and that you have all of the relevant `frames` setup for your application.

If you are new to PlayRM's messaging feature, please refer to <a href="http://integration.playnomics.com" target="_blank">integration documentation</a>.

Once you have all of your frames created with their associated `<PLAYRM-FRAME-ID>`s, you can start the integration process.

## SDK Integration

To work with, the messaging you'll need to import the appropriate header files into your UIViewController header file. Notice that your UIViewController, 

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
            <td>frameId</td>
            <td>string</td>
            <td>Unique identifier for the frame.</td>
        </tr>
    </tbody>
</table>

Frames are loaded asynchronously to keep your game responsive. The `createFrameWithId` call begins the frame loading process. However, until you call `start` on the frame, the frame will not be drawn in the UI. This gives you control over when a frame will appear. Frames are destroyed on a ViewController transition or when closed.

In the example below, we initialize the frame when view is visible and then show it in another event delegate.

In practice, a frame can be loaded in a variety of ways.

```objectivec
#import "ViewController.h"

@implementation ViewController
{
    - (void)viewDidLoad
    {
        PlaynomicsMessaging *messaging = [PlaynomicsMessaging sharedInstance];
        frame = [messaging createFrameWithId: @"<PLAY-FRAME-ID>"];
    }

    -void someOtherEvent()
    {
        [frame.start];
    }
}
```

## Using Code Callbacks

Depending on your configuration, a variety of actions can take place when a frame's message is pressed or clicked:

* Redirect the player to a web URL in Safari
* Fire a code callback in your game
* Or in the simplest case, just close, provided that the **Close Button** has been configured correctly.

All of this setup, takes place at the the time of the messaging campaign configuration. However, all code callbacks need to be configured before PlayRM can interact with them. The SDK uses a NSObject delegate, the `respondsToSelector` method to find and verify the delegate callback method exists, and finally uses `performSelector` to run your callback. 

* The callback cannot accept any parameters. 
* It should return `void`.

**The code callback will not fire if the Close button is pressed.**

Here are three common use cases for frames and a messaging campaigns

* [Game Start Frame](#game-start-frame)
* [Event Driven Frame - Open the Store](#event-driven-frame-open-the-store) for instance, when the player is running low on premium currency
* [Event Driven Frame - Level Completion](#event-driven-drame-level-completion)

For each of the examples, we will attach our callback to the ViewController implementation from the previous information

```objectivec
PlaynomicsMessaging *messaging = [PlaynomicsMessaging sharedInstance];
//this is setting the delegate to the UIViewController class that the frame is running in
//all callbacks will be configured here
messaging.delegate = self;
```

### Game Start Frame

In this use-case, we want to configure a frame that is always shown to players when they start playing a new game. The message shown to the player may change based on the desired segments:

<table>
    <thead>
        <tr>
            <th >
                Segment
            </th>
            <th>
                Priority
            </th>
            <th>
                Code Callback
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
#import <UIKit/UIKit.h>
#import "PlaynomicsSession.h"
#import "PlaynomicsFrame.h"
#import "PlaynomicsMessaging.h"

@interface ViewController : UIViewController <UITextFieldDelegate>
{
    @private
    PlaynomicsFrame frame;

    //...
    //...
}

//grant 10 MonsterBucks
- (void) grant10MonsterBucks;

//grant 50 MonsterBucks
- (void) grant50MonsterBucks;

//grant a bazooka
- (void) grantBazooka;

//...
//...

@end
```

The related messages would be configured in the Control Panel to use this callback by placing this in the **Target URL** for each message :

* **At-Risk Message** : `pnx://grant50MonsterBucks`
* **Lapsed 7 or more days** : `pnx://grant10MonsterBucks`
* **Default** : `pnx://grantBazooka`

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
                Code Callback
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

```objectiveC
#import <UIKit/UIKit.h>
#import "PlaynomicsSession.h"
#import "PlaynomicsFrame.h"
#import "PlaynomicsMessaging.h"

@interface ViewController : UIViewController <UITextFieldDelegate>
{
    @private
    PlaynomicsFrame frame;

    //...
    //...
}

//open the store
- (void) openStore;

//...
//...

@end
```

The Default message would be configured in the Control Panel to use this callback by placing this in the **Target URL** for the message : `pnx://ClickHandler.openStore`.

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
                Code Callback
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

```objectiveC
#import <UIKit/UIKit.h>
#import "PlaynomicsSession.h"
#import "PlaynomicsFrame.h"
#import "PlaynomicsMessaging.h"

@interface ViewController : UIViewController <UITextFieldDelegate>
{
    @private
    PlaynomicsFrame frame;

    //...
}

//grant mana
- (void) grantMana;

//...

@end
```

The related messages would be configured in the Control Panel to use this callback by placing this in the **Target URL** for each message :

* **Non-monetizers, in their 5th day of game play** : `HTTP URL for Third Party Ad`
* **Default** : `pnx://grantMana`

Push Notifications
==================

## Registering for PlayRM Push Messaging

To get started with PlayRM Push Messaging, your app will need to register with Apple to receive push notifications. Do this by calling the `registerForRemoteNotificationTypes` method on UIApplication.

```objectivec
@implementation AppDelegate

//...

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    const long long applicationId = <APPID>;
    [PlaynomicsSession startWithApplicationId:applicationId];

    //enable notifications
    UIApplication *app = [UIApplication sharedApplication];
    [app registerForRemoteNotificationTypes: (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];

    //...
}
```

Once the player, authorizes push notifications from your app, you need to provide Playnomics with player's device token
```
@implementation AppDelegate

//...

-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
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
-(void) application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
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

To do this, insert this code snippet in the `applicationWillResignActive` methpod of your `UIAppDelegate`

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
