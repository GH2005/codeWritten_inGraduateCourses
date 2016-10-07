//////////////////////////////////////////////////////////////////////////////////////////////////////////
// csci-6509 project, PT-01, data preparation
//////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;
using Tweetinvi;

namespace proj_nlp
{
    class Program
    {
		private const int MIN_TWEET_LEN = 140;
        static void Main(string[] args)
        {
			Console.Title = "CSCI-6509 Project, PT-01, Data Preparation";
			Console.WriteLine("Will collect tweets that are no less than {0} chars long.",
				Program.MIN_TWEET_LEN);
            //
            // credentials for current thread and the entire application
            //
            Auth.SetUserCredentials(
                "KqzxfjooejRlapQ5sspo7ZeTa",
                "JO3mBoohNPHtYHjSF0aUalo7HyvJQs6q29slguvJrhwQzHdIr6",
                "4895136316-dce64J8Z19Jo3LHsDFiDAQULGxcNz6CaKnJfXtM",
                "hMf0ck1N7jz2pZdbPpB0f7DBOWAjxScPdantD8F7pzGC9");
            //
            // use ratelimit tracking
            //
            RateLimit.RateLimitTrackerOption = Tweetinvi.Core.RateLimitTrackerOptions.TrackOnly;
            TweetinviEvents.QueryBeforeExecute += (sender, arg_query) =>
            {
                var rate_limit = RateLimit.GetQueryRateLimit(arg_query.QueryURL);
                if (rate_limit == null) Console.WriteLine("Not a rate-limited request.");
                else // a limited request
                {
                    if (rate_limit.Remaining > 0) // available
                    {
                        Console.WriteLine("Remaining limits: {0}.", rate_limit.Remaining);
                        return;
                    }
                    // not available currently, halt the thread
                    // is the ResetDateTime the time span or the moment?
                    Console.WriteLine("Waiting for RateLimits until: {0}",
						rate_limit.ResetDateTime.ToLongTimeString());
                    Thread.Sleep((int)rate_limit.ResetDateTimeInMilliseconds);
                }
            };
            //
            // create and configure the stream
            //
            var stream = Tweetinvi.Stream.CreateFilteredStream();
            stream.AddTweetLanguageFilter(Tweetinvi.Core.Enum.Language.English);
            stream.FilterLevel = Tweetinvi.Core.Interfaces.Streaminvi.Parameters.StreamFilterLevel.Low;
            stream.LimitReached += (sender, arg_lmt) =>
            {
                Console.WriteLine("Stream attempted to access more than 1%; not received: {0}.",
                    arg_lmt.NumberOfTweetsNotReceived);
            };
            // handle stream stop
            // AutoResetEvent are = new AutoResetEvent(false); // really OK?
            stream.StreamStopped += (sender, arg_stop) =>
            {
                Console.WriteLine("The stream has stopped.");
                if (arg_stop.Exception != null) Console.WriteLine(arg_stop.Exception);
                if (arg_stop.DisconnectMessage != null) Console.WriteLine(arg_stop.DisconnectMessage);
                // are.Set();
            };
            stream.DisconnectMessageReceived += (sender, arg_dis) =>
            {
                Console.WriteLine(arg_dis.DisconnectMessage);
            };
            stream.StreamStarted += (sender, arg_stt) =>
            {
                Console.WriteLine("Stream started; press Ctrl+C to stop it.");
            };
			// Ctrl+C is used to stop the stream.
			Console.CancelKeyPress += (sender, arg_canc) =>
			{
				stream.StopStream();	// an outer variable that has an extended life cycle
				arg_canc.Cancel = true;
			};
			//
			// process every received tweet
			//
			int count = 0;
			int reqr_c = 0;
			DateTime start_time = DateTime.Now;
			string track = null;
			StreamWriter file = null;
			stream.MatchingTweetReceived += (sender, arg_twt) =>
			{
				var tweet = arg_twt.Tweet;
				var loc = tweet.CreatedBy.Location;
				string txt = tweet.Text;
				if (txt.Length >= MIN_TWEET_LEN && loc != null)
				{ // eligible tweets; trim commas and quote signs off and shorten ws chars
					txt = Regex.Replace(txt, "[\",]", " ");
					txt = Regex.Replace(txt, "\\s+", " ");
					loc = Regex.Replace(loc, "[\",]", " -");
					loc = Regex.Replace(loc, "\\s+", " ");
					Console.WriteLine("{0},\"{1}\",\"{2}\"", count, txt, loc);
					file.WriteLine("{0},\"{1}\",\"{2}\"", count++, txt, loc);
					if (reqr_c > 0)
					{
						double perc = Convert.ToDouble(100 * count) / Convert.ToDouble(reqr_c) ;
						TimeSpan ts = DateTime.Now.Subtract(start_time);
						Console.Title = "(" + perc.ToString("F1") + " %) " + count + "/" + reqr_c
							+ " collected, Tweets of \"" + track + "\", " + ts.ToString(@"hh\:mm\:ss")
							+ " elapsed";
					}
					else
					{
						TimeSpan ts = DateTime.Now.Subtract(start_time);
						Console.Title = count + " collected, Tweets of \"" + track + "\", "
							+ ts.ToString(@"hh\:mm\:ss") + " elapsed";
					}
				}
				if (reqr_c > 0 && count >= reqr_c) stream.StopStream();
			};
			//stream.JsonObjectReceived += (sender, arg_json) =>
			//{
			//	Console.WriteLine(arg_json.Json);
			//};
			//
			// start stream using the user-input keywords
			//
			Console.WriteLine("Input \"start\" to start a new filtered stream or \"exit\" to quit:");
            string user_in = Console.ReadLine();
            while (user_in != "exit")
            {
                if (user_in == "start")
                {
                    // ask the user for keywords.
                    Console.Write("Input whitespace-separated keywords for getting filtered tweets; ");
                    Console.WriteLine("multiple keywords are AND-connected:");
                    track = Console.ReadLine();
                    stream.AddTrack(track);
					// ask the user for number of tweets needed
					Console.WriteLine("Input the number of tweets to  (0 for unlimited):");
					reqr_c = int.Parse(Console.ReadLine());
					count = 0;
					string file_nm = string.Format("{0}_{1}", track, DateTime.Now.ToString("MMdd_HHmm"));
					file = new StreamWriter(file_nm);
					file.WriteLine("index,text,location");
					// start the stream and wait until it stops;
					start_time = DateTime.Now;
					stream.StartStreamMatchingAllConditions();  // a synchronous method
                    // are.WaitOne();
                    // must remove the previous track
                    stream.RemoveTrack(track);
					file.Flush(); file.Close();
					FileInfo fi = new FileInfo(file_nm);
					file_nm = file_nm + string.Format("_{0}_{1}.csv", DateTime.Now.ToString("MMdd_HHmm"),
						count);
					fi.MoveTo(file_nm);
					Console.Title = "CSCI-6509 Project, PT-01, Data Preparation";
				}
                Console.WriteLine("Input \"start\" to start a new filtered stream or \"exit\" to quit:");
                user_in = Console.ReadLine();
            }
        }
    }
}