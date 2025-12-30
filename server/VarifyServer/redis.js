// 引入配置模块，获取Redis连接配置信息
const config_module = require('./config')
// 引入ioredis库，这是一个功能强大的Redis客户端库
const Redis = require("ioredis");

// 创建Redis客户端实例
// Redis是一个内存数据库，常用于缓存、会话存储等场景
// 这里用于存储验证码等临时数据
const RedisCli = new Redis({
  host: config_module.redis_host,       // Redis服务器主机名，如 'localhost' 或远程IP
  port: config_module.redis_port,       // Redis服务器端口号，默认为 6379
  password: config_module.redis_passwd, // Redis密码，用于身份验证
});

/**
 * 监听Redis连接错误事件
 * 当Redis连接出现问题时（如网络中断、认证失败等），会触发此事件
 * 一旦发生错误，会打印错误信息并关闭连接
 */
RedisCli.on("error", function (err) {
  console.log("RedisCli connect error");
  // 关闭Redis连接，释放资源
  RedisCli.quit();
});

/**
 * 根据key获取Redis中存储的value
 * 这是一个异步函数，用于从Redis数据库中读取指定键的值
 * 
 * @param {string} key - 要查询的键名
 * @returns {string|null} - 如果键存在，返回对应的值；如果键不存在或发生错误，返回null
 * 
 * 使用场景：获取验证码、用户会话等临时数据
 */
async function GetRedis(key) {

    try{
        // 使用await等待异步操作完成，从Redis中获取key对应的value
        const result = await RedisCli.get(key)
        
        // 如果返回null，说明该key不存在
        if(result === null){
          console.log('result:','<'+result+'>', 'This key cannot be find...')
          return null
        }
        
        // 成功获取到值，打印日志并返回结果
        console.log('Result:','<'+result+'>','Get key success!...');
        return result
    }catch(error){
        // 捕获异常，打印错误信息并返回null
        console.log('GetRedis error is', error);
        return null
    }

  }

/**
 * 查询Redis中是否存在指定的key
 * 与GetRedis不同，这个函数只检查key是否存在，不获取具体的值
 * 
 * @param {string} key - 要查询的键名
 * @returns {number|null} - 如果键存在返回1，不存在返回null，发生错误也返回null
 * 
 * 使用场景：在获取值之前先检查key是否存在，避免不必要的操作
 */
async function QueryRedis(key) {
    try{
        // exists方法返回0表示不存在，返回1表示存在
        const result = await RedisCli.exists(key)
        
        // 判断该key是否存在，result为0表示不存在
        if (result === 0) {
          console.log('result:<','<'+result+'>','This key is null...');
          return null
        }
        
        // key存在，返回结果（1）
        console.log('Result:','<'+result+'>','With this value!...');
        return result
    }catch(error){
        // 捕获异常，打印错误信息并返回null
        console.log('QueryRedis error is', error);
        return null
    }

  }

/**
 * 设置key-value键值对，并设置过期时间
 * 这是验证码系统的核心功能之一，用于存储临时数据并自动过期
 * 
 * @param {string} key - 要设置的键名，例如：用户邮箱作为key
 * @param {string} value - 要存储的值，例如：验证码内容
 * @param {number} exptime - 过期时间（单位：秒），例如：300表示5分钟后过期
 * @returns {boolean} - 设置成功返回true，失败返回false
 * 
 * 使用场景：存储验证码，设置5分钟有效期；存储会话token等
 * 为什么需要过期时间？
 * 1. 自动清理过期数据，节省内存
 * 2. 验证码有时效性，过期后自动失效，提高安全性
 */
async function SetRedisExpire(key,value, exptime){
    try{
        // 第一步：设置键和值，将数据存入Redis
        await RedisCli.set(key,value)
        
        // 第二步：设置过期时间（以秒为单位）
        // 到达过期时间后，Redis会自动删除这个key
        await RedisCli.expire(key, exptime);
        
        // 操作成功，返回true
        return true;
    }catch(error){
        // 捕获异常，打印错误信息并返回false
        console.log('SetRedisExpire error is', error);
        return false;
    }
}

/**
 * 退出函数 - 关闭Redis连接
 * 当应用程序关闭或不再需要Redis连接时调用此函数
 * quit()方法会等待所有待处理的命令完成后再关闭连接
 * 
 * 使用场景：
 * 1. 应用程序正常退出时
 * 2. 发生错误需要释放资源时
 * 3. 测试完成后清理连接
 */
function Quit(){
    // 优雅地关闭Redis连接，等待所有命令执行完毕
    RedisCli.quit();
}

// 导出模块的所有公共函数，供其他文件调用
// 这样其他文件就可以通过 require('./redis') 来使用这些Redis操作函数
module.exports = {
    GetRedis,          // 获取键值
    QueryRedis,        // 查询键是否存在
    Quit,              // 关闭连接
    SetRedisExpire,    // 设置带过期时间的键值对
}