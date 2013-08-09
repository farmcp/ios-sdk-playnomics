//  Playnomics PlayRM SDK
//  PlaynomicsSession.h
//
//  Copyright (c) 2012 Playnomics. All rights reserved.
//
//  Please see http://integration.playnomics.com for instructions
//  Please contact support@playnomics.com for assistance

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


/**
 *  PNAPIResult
 *
 *  Possible results for PlaynomicsSession messages
 */
typedef enum {
    PNAPIResultNotSent,
    PNAPIResultSent,
    PNAPIResultQueued,
    PNAPIResultSwitched,
    PNAPIResultStopped,
    PNAPIResultAlreadyStarted,
    PNAPIResultAlreadySwitched,
    PNAPIResultAlreadyStopped,
    PNAPIResultSessionResumed,
    PNAPIResultStartNotCalled,
    PNAPIResultNoInternetPermission,
    PNAPIResultMissingReqParam,
    PNAPIResultFailUnkown
} PNAPIResult;


/**
 *  PNResponseType
 *
 *  Possible responses for the invitations/SocialEvents
 */
typedef enum {
    PNResponseTypeAccepted
} PNResponseType;

/**
 *  PNTransactionType
 *
 *  Possible Transaction Types
 */
typedef enum {
    PNTransactionBuyItem,
    PNTransactionSellItem,
    PNTransactionReturnItem,
    PNTransactionBuyService,
    PNTransactionSellService,
    PNTransactionReturnService,
    PNTransactionCurrencyConvert,
    PNTransactionInitial,
    PNTransactionFree,
    PNTransactionReward,
    PNTransactionGiftSend,
    PNTransactionGiftReceive
} PNTransactionType;

/**
 *  PNCurrencyCategory
 *
 *  Possible currency Categories for transactions
 */
typedef enum {
    PNCurrencyCategoryReal,
    PNCurrencyCategoryVirtual
} PNCurrencyCategory;

/**
 *  PNCurrencyType
 *
 *  Possible currency Types for transactions
 */
typedef enum {
    PNCurrencyUSD,
    PNCurrencyFBC,
    PNCurrencyOFD,
    PNCurrencyOFF
} PNCurrencyType;

/**
 *  PNUserInfoType
 *
 *  Possible User Info requests types
 */
typedef enum {
    PNUserInfoTypeUpdate
} PNUserInfoType;

/**
 *  PNUserInfoSex
 *
 *  Possible Sex types.
 *  Sent in a user update request.
 */
typedef enum {
    PNUserInfoSexMale,
    PNUserInfoSexFemale,
    PNUserInfoSexUnknown
} PNUserInfoSex;

/**
 *  PNUserInfoSource
 *
 *  Where the User Info comes from.
 *  Sent in a user update request.
 */
typedef enum {
    PNUserInfoSourceAdwords,
    PNUserInfoSourceDoubleClick,
    PNUserInfoSourceYahooAds,
    PNUserInfoSourceMSNAds,
    PNUserInfoSourceAOLAds,
    PNUserInfoSourceAdbrite,
    PNUserInfoSourceFacebookAds,
    PNUserInfoSourceGoogleSearch,
    PNUserInfoSourceYahooSearch,
    PNUserInfoSourceBingSearch,
    PNUserInfoSourceFacebookSearch,
    PNUserInfoSourceApplifier,
    PNUserInfoSourceAppStrip,
    PNUserInfoSourceVIPGamesNetwork,
    PNUserInfoSourceUserReferral,
    PNUserInfoSourceInterGame,
    PNUserInfoSourceOther
} PNUserInfoSource;

@interface PlaynomicsSession : NSObject

/**
 * Change User.
 *
 * @param userId
 *            the user id
 * @return the API Result
 */
+ (PNAPIResult) changeUserWithUserId: (NSString *) userId;

/**
 * Start.
 *
 * @param applicationId
 *            the application id
 * @param userId
 *            the user id
 * @return the API Result
 */
+ (PNAPIResult) startWithApplicationId:(signed long long) applicationId
                                userId: (NSString *) userId;

/**
 * Start.
 *
 * @param applicationId
 *            the application id
 * @return the API Result
 */
+ (PNAPIResult) startWithApplicationId:(signed long long) applicationId;

/**
 * Stop.
 *
 * @return the API Result
 */
+ (PNAPIResult) stop;

/**
 * Enables testing mode
 * @param testMode: use true to enable sending data to test servers
 */
+ (void) setTestMode: (bool) testMode;

/* 
 Check if the SDK is in test mode or not.
*/
+ (bool) getTestMode;


+ (NSString*) getOverrideMessagingUrl;
+ (void) setOverrideMessagingUrl: (NSString*) url;

+ (NSString*) getOverrideEventsUrl;
+ (void) setOverrideEventsUrl: (NSString*) url;

+ (NSString*) getSDKVersion;
@end

@interface PlaynomicsSession (Events)
/**
 * User info.
 *
 * @param type
 *            the type
 * @param country
 *            the country
 * @param subdivision
 *            the subdivision
 * @param sex
 *            the sex
 * @param birthday
 *            the birthday
 * @param source
 *            the source must be a PNUserInfoSource
 * @param sourceCampaign
 *            the source campaign
 * @param installTime
 *            the install time
 * @return the API Result
 */
+ (PNAPIResult) userInfoForType: (PNUserInfoType) type
                        country: (NSString *) country
                    subdivision: (NSString *) subdivision
                            sex: (PNUserInfoSex) sex
                       birthday: (NSDate *) birthday
                         source: (PNUserInfoSource) source
                 sourceCampaign: (NSString *) sourceCampaign
                    installTime: (NSDate *) installTime;
/**
 * User info.
 *
 * @param type
 *            the type
 * @param country
 *            the country
 * @param subdivision
 *            the subdivision
 * @param sex
 *            the sex
 * @param birthday
 *            the birthday
 * @param source
 *            the source can be any String
 * @param sourceCampaign
 *            the source campaign
 * @param installTime
 *            the install time
 * @return the API Result
 */
+ (PNAPIResult) userInfoForType: (PNUserInfoType) type
                        country: (NSString *) country
                    subdivision: (NSString *) subdivision
                            sex: (PNUserInfoSex) sex
                       birthday: (NSDate *) birthday
                 sourceAsString: (NSString *) source
                 sourceCampaign: (NSString *) sourceCampaign
                    installTime: (NSDate *) installTime;

/**
 * Session start.
 *
 * @param sessionId
 *            the session id
 * @param site
 *            the site
 * @return the API Result
 */
+ (PNAPIResult) sessionStartWithId: (signed long long) sessionId
                              site: (NSString *) site;

/**
 * Session end.
 *
 * @param sessionId
 *            the session id
 * @param reason
 *            the reason
 * @return the API Result
 */
+ (PNAPIResult) sessionEndWithId: (signed long long) sessionId
                          reason: (NSString *) reason;

/**
 * Game start.
 *
 * @param instanceId
 *            the instance id
 * @param sessionId
 *            the session id
 * @param site
 *            the site
 * @param type
 *            the type
 * @param gameId
 *            the game id
 * @return the API Result
 */
+ (PNAPIResult) gameStartWithInstanceId: (signed long long) instanceId
                              sessionId: (signed long long) sessionId
                                   site: (NSString *) site
                                   type: (NSString *) type
                                 gameId: (NSString *) gameId;

/**
 * Game end.
 *
 * @param instanceId
 *            the instance id
 * @param sessionId
 *            the session id
 * @param reason
 *            the reason
 * @return the API Result
 */
+ (PNAPIResult) gameEndWithInstanceId: (signed long long) instanceId
                            sessionId: (signed long long) sessionId
                               reason: (NSString *) reason;

/**
 * Transaction.
 *
 * @param transactionId
 *            the transaction id
 * @param itemId
 *            the item id
 * @param quantity
 *            the quantity
 * @param type
 *            the type
 * @param otherUserId
 *            the other user id
 * @param currencyType
 *            the currency type must be of PNCurrencyType
 * @param currencyValue
 *            the currency value
 * @param currencyCategory
 *            the currency category
 * @return the API Result
 */
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
                     currencyType: (PNCurrencyType) currencyType
                    currencyValue: (double) currencyValue
                 currencyCategory: (PNCurrencyCategory) currencyCategory;

/**
 * Transaction.
 *
 * @param transactionId
 *            the transaction id
 * @param itemId
 *            the item id
 * @param quantity
 *            the quantity
 * @param type
 *            the type
 * @param otherUserId
 *            the other user id
 * @param currencyType
 *            the currency type, can be any string
 * @param currencyValue
 *            the currency value
 * @param currencyCategory
 *            the currency category
 * @return the API Result
 */
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
             currencyTypeAsString: (NSString *) currencyType
                    currencyValue: (double) currencyValue
                 currencyCategory: (PNCurrencyCategory) currencyCategory;

/**
 * Transaction.
 *
 * @param transactionId
 *            the transaction id
 * @param itemId
 *            the item id
 * @param quantity
 *            the quantity
 * @param type
 *            the type
 * @param otherUserId
 *            the other user id
 * @param currencyTypes
 *            the currency types NSNumber (intValue of PLCurrencyType) or NSString
 * @param currencyValues
 *            the currency values
 * @param currencyCategories
 *            the currency categories
 * @return the API Result
 */
+ (PNAPIResult) transactionWithId: (signed long long) transactionId
                           itemId: (NSString *) itemId
                         quantity: (double) quantity
                             type: (PNTransactionType) type
                      otherUserId: (NSString *) otherUserId
                    currencyTypes: (NSArray *) currencyTypes
                   currencyValues: (NSArray *) currencyValues
               currencyCategories: (NSArray *) currencyCategories;

/**
 * Invitation sent.
 *
 * @param invitationId
 *            the invitation id
 * @param recipientUserId
 *            the recipient user id
 * @param recipientAddress
 *            the recipient address
 * @param method
 *            the method
 * @return the API Result
 */
+ (PNAPIResult) invitationSentWithId: (signed long long) invitationId
                     recipientUserId: (NSString *) recipientUserId
                    recipientAddress: (NSString *) recipientAddress
                              method: (NSString *) method;

/**
 * Invitation response.
 *
 * @param invitationId
 *            the invitation id
 * @param response
 *            the response
 * @return the API Result
 */
+ (PNAPIResult) invitationResponseWithId: (signed long long) invitationId
                         recipientUserId: (NSString *) recipientUserId
                            responseType: (PNResponseType) responseType;

/**
 * Milestone
 *
 * @param milestoneId
 *            the milestone id
 * @param milestoneName
 *            the milestone name
 * @return the API Result
 */
+ (PNAPIResult) milestoneWithId: (signed long long) milestoneId
                        andName: (NSString *) milestoneName;


/**
 * PushNotificationEnabled
 *
 * @param deviceToken
 *            the device token
 * @return the API Result
 */
+ (PNAPIResult) enablePushNotificationsWithToken:(NSData*)deviceToken;

/**
 * pushNotificationsWithPayload
 *
 * @param payload
 *            the dictionary that was pushed to device
 * @return the API Result
 */
+ (void) pushNotificationsWithPayload:(NSDictionary*)payload ;

/**
 * errorReport
 *
 * @param errorDetails
 *            the PNErrorEvent object with error details
 * @return the API Result
 */
+ (PNAPIResult) errorReport:(id)errorDetails;

+(NSString*)stringForTrimmedDeviceToken:(NSData*)deviceToken;

@end

@interface PNApplication : UIApplication<UIApplicationDelegate>
- (void) sendEvent:(UIEvent *)event;
@end
