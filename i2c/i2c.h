/*
 * i2c.h
 *
 * Copyright (C) 2015 Miguelsoft NETwork
 *
 *  Created on: 28 may. 2015
 *      Author: Miguel Angel Abella Gonzalez
 *
 * See the file LICENSE, included with the source code, for details about
 * copyright and restrictions.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#ifndef DEVICES_I2C_H_
#define DEVICES_I2C_H_

#include <linux/i2c-dev.h>


/*
 * Define a data structure for accessing I2C devices.
 *
 * This structure holds the I2C port for the master/board and the address of the
 * slave device.
 */
typedef struct i2c_ctl {
		int port;          /* The I2C interface port (/dev/i2c-#) */
		int port_fd;       /* The I2C interface port file descriptor */
		int slave_address; /* The I2C slave address (rtc@0x68) */
} i2c_ctl_t;

/*
 * Initialize an I2C device.
 *
 * The internal field "port" must be set with the desired port number before
 * calling this function.
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 */
int i2c_init(i2c_ctl_t * i2c);
/*
 * Release an I2C device.
 *
 * All calls to any i2c_* function will fail after releasing the device.
 * A subsequent call to i2c_init may succeed.
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 */
int i2c_release(i2c_ctl_t * i2c);

/*
 * Selects a slave device attached to the I2C.
 *
 * The "slave address" field must be provided for this call to succeed.
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 * Errno is also returned as "-errno".
 */
int i2c_select_slave(const i2c_ctl_t i2c);

/*
 * Reads data from a slave device.
 *
 * The slave register address to be read must be passed as the first argument.
 *
 * The return value is -1 on error or the actual data on success.
 */
int i2c_read(const i2c_ctl_t i2c, __u8 i2c_slave_register);
/*
 * Writes data to a slave device.
 *
 * The slave register address in which to write must be passed in "i2c_register"
 * The buffer must be a valid buffer able to hold "len" elements.
 *
 * Returns the number of written bytes into the buffer.
 */
int i2c_write(const i2c_ctl_t i2c, __u8 i2c_register, const __u8 * buffer, __u8 len);

int i2c_write_byte(const i2c_ctl_t i2c, __u8 i2c_register, const __u8 data);

#endif /* DEVICES_I2C_H_ */
