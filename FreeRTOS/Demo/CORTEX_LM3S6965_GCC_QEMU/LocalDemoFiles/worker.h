/*
 * worker.h
 *
 *  Created on: 4 Oct 2023
 *      Author: jelao
 */

#ifndef LOCALDEMOFILES_WORKER_H_
#define LOCALDEMOFILES_WORKER_H_

void WorkerTask( void *pvParameters );

void initWorkers();

void stopWorkers();

#endif /* LOCALDEMOFILES_WORKER_H_ */
