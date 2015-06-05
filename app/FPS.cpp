#include "fps.h"
#ifdef __WIN32__
int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#endif // __WIN32__


FPS_Helper::FPS_Helper()
{
    this->m_uFixedFPSInterval = 32;
    this->m_iFPSBegin = 0;
    this->m_iTimeUpdate = 0;
    this->m_uFPS = 0;
    this->m_uFPSCounter = 0;
    m_onFpsUpdate = NULL;
}

void FPS_Helper::SetFixedFPS(unsigned int fps)
{
    this->m_uFixedFPSInterval = 1000.0f/(float)fps;
}

bool FPS_Helper::Tick()
{
    static long int msec_curr = 0;
    static timeval t_curr = {0};
    gettimeofday(&t_curr,NULL); // & 0xffff 是因为乘上1000会溢出，只保留低位即可
    msec_curr = (t_curr.tv_sec & 0xffff) * 1000 + t_curr.tv_usec / 1000;
    long int msec_diff = msec_curr - this->m_iTimeUpdate;
    long int msec_fps_diff = msec_curr - this->m_iFPSBegin;

    if(msec_fps_diff >= 1000)
    {
        //printf("fps : %d \n",fps);
        this->m_uFPS = this->m_uFPSCounter;
        this->m_uFPSCounter = 0;
        this->m_iFPSBegin = msec_curr;
        // fps更新回调
        if(m_onFpsUpdate)
            m_onFpsUpdate(m_uFPS);
    }

    if(msec_diff > this->m_uFixedFPSInterval)
    {
        // 如果期间有2倍以上间隔应该立即刷新，刷新时间就是当前时间
        // 如果没有2倍以上，则计算出对应的时间
        if(msec_diff < this->m_uFixedFPSInterval * 2)
        {
            m_iTimeUpdate = m_iTimeUpdate + m_uFixedFPSInterval;
        }
        else
        {
            m_iTimeUpdate = msec_curr;
        }
        m_uFPSCounter++;
        return true;
    }
    else
    {
        return false;
    }
}

void FPS_Helper::SetFpsCallback(FPS_UPDATE_CALLBACK callback)
{
    this->m_onFpsUpdate = callback;
}
