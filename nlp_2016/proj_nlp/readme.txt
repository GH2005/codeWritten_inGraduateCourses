1. Data Collection

To collect Tweets of certain topics directly from Twitter, programming with Twitter APIs is necessary. We needed to learn about features of Twitter APIs and find a platform to implement the program. The major aspects are as follows.

1) Twitter APIs

Twitter offers mainly two kinds of APIs: REST APIs and Streaming APIs. REST APIs are suitable for building a Twitter client, which allows developers to implement functions like authoring a new Tweet, reading author profile and follower data, and more [1]. Streaming APIs give developers low latency access to Twitter’s global stream of Tweet data. A proper implementation of a streaming client will be pushed messages indicating Tweets and other events have occurred [1]. Considering our purpose of the study, Streaming APIs would be appropriate.

The Streaming APIs can be divided into three categories: Public Streams, User Streams, and Site Streams. Public Streams provide public data flowing through Twitter that is suitable for following specific users or topics, and data mining [1]. User Streams contain roughly all data related to one user, and Site Streams are multi-user version of User Streams. Obviously, we chose Public Streams filtered by tracks to collect our data. “Track” is the term to denote a Twitter parameter, such as keywords used to filter tweets for certain topics.

2) Coding Platform

For this section, we chose to use Microsoft .NET as the development framework for implementation. 

We used C# as the programming language, together with a Twitter library called Tweetinvi [2]. Tweetinvi is a library that encapsulates Twitter APIs in C# style. It is available in Visual Studio via Nuget: Install-Package TweetinviAPI.

3) Things to be done before data streaming

Twitter requires applications to be authenticated to invoke API requests. It employs a protocol 
called OAuth, which requires the application to show the credentials that are stored as OAuth Tokens to the server. To obtain the credentials, we applied for a Twitter Application on apps.twitter.com and got four keys or tokens: Consumer Key (API Key), Consumer Secret (API Secret), Access Token and Access Token Secret. Since the details of the credentials are not very relevant to our project, we will not cover them in this report. We set the credentials properly in the code, and continued.

Next, we created a filtered public stream and configured it. The language filter was naturally set to English. We have to set the filter level to “low”, because later tests showed that nearly no tweets were returned when it was “medium” or “high”. And the last thing to do before actually starting the stream was adding Tracks (keywords) to it. Twitter and Tweetinvi allow a public stream to be bounded with multiple Tracks arranged by logical relations such as “AND” and “OR”. In this program, users can enter space-separated Tracks each time they want to start a new stream, and the Tracks will be “AND” connected.

4) Collecting Tweets

Before streaming, it’s better to know the fact that Twitter will only return 1% of the total tweets posted in real time. “With Filtered Streams, this rule means that if your filters criteria are too wide and would return more than 1% of the total tweets posted on Twitter, the Stream API will not return all the tweets.” [2]

The C# method we used to start streaming is stream.StartStreamMatchingAllConditions();. It is a synchronous method, which means it will dominate the thread who called it until the stream stops. .NET events were used to handle all kinds of information generated. Table 1 shows some events handled in this program. In particular, the only keyboard shortcut event, Ctrl+C, is used to let the user stop the stream manually.

A returned tweet is in JSON format shown in Table 2. In our project, we cared only about two fields, “text” and “user.location”. The former is the text message, and the latter is the geographical location claimed by the creator of this tweet, which means it may not be a standard location phrase. Only tweets with a message that was no less than 140 characters and a nonempty user location string were accepted as our raw data. We could access and evaluate these fields by approaches provided by Tweetinvi library.

Preliminary Changes to the Tweets
For both the two fields of every received tweet, quotation marks and commas were removed, and consecutive Whitespace Characters were replaced with one single blank character. We did this using .NET regular expression facilities. Further changes on Tweets other than ones mentioned above could also be applied to meet requirements of following sections.

Data Storage

Every time a user runs a stream and when it stops, the received data will be automatically saved into a file of DSV format. The filename will follow this pattern: Tracks_StartDate_StartTime_EndDate_EndTime_NumberOfTweets.csv.

