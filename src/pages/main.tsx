import { useEffect, useState } from "react";
import { fetchdata } from "../api/dataApi";

const Home = () => {
    const [iotDatas, setIotData] = useState([]);
    const [isLoading, setIsLoading] = useState(false);
    const [feedback, setFeedback] = useState(false);
    const [count, setCount] = useState(0);
    const options = {
        day:"2-digit",
        month:"long",
        year:"numeric",
        hour:"2-digit",
        minute:"2-digit",
        hour12: true
    };

    const [lastUpdated, setLastUpdated] = useState(null);

useEffect(() => {
    const fetchData = async () => {
        try {
            const res = await fetchdata();
            if (res.data && res.data.data && Array.isArray(res.data.data)) {
                    setIotData(res.data.data);
                    setCount(res.data.count);
                } else if (Array.isArray(res.data)) {
                    // Fallback for direct array response
                    setIotData(res.data);
                    setCount(res.data.length);
                }        
            setLastUpdated(new Date());
        } 
        catch (e) {
            console.error(e);
            setFeedback(e || "Error occurred");
        }
    };

    // Initial load
    setIsLoading(true);
    setCount(0);
    fetchData().finally(() => setIsLoading(false));
    
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
}, []);

  return (
    <div className="bg-white pt-20">
        <div className="max-w-7xl m-auto px-4 md:px-6 lg:px-8">
            <div className="text-right mb-4 flex justify-between ">
                <h2 className="font-semibold">IOTDATA from esp32</h2>
                {count > 1000 && (<p>{count}</p>)}
                {lastUpdated && <small>Last updated: {lastUpdated.toLocaleTimeString()}</small>}
            </div>
                <div >
                    {iotDatas.map((iotData)=>(
                        <div key={iotData.id} className="mb-2 border border-gra-500 bg-white rounded-md shadow-md p-6 ">
                            <h2>Temperature: <span className="font-semibold">{iotData.temperature}</span> °C</h2>
                            <h2>Humidity: <span className="font-semibold">{iotData.humidity}</span></h2>
                            <p>brightnessPercent: <span className="font-semibold">{iotData.brightnessPercent}</span></p>
                            <p>Motion: {iotData.detected}</p>
                            <p>{new Date(iotData.date).toLocaleString('en-US', options)}</p>
                        </div>
                    ))}
                </div>
        </div>
    </div>
  )
}

export default Home;